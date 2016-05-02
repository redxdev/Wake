#include "bindings/luaassets.h"
#include "bindings/luamesh.h"
#include "moduleregistry.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <fstream>
#include <snappy.h>
#include <sstream>

namespace wake
{
    namespace bindings
    {
        int loadModel(lua_State* L)
        {
            const char* path = luaL_checkstring(L, 1);

            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path,
                                                     aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

            if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cout << "loadModel error: " << importer.GetErrorString() << std::endl;
                lua_pushnil(L);
                return 1;
            }

            std::vector<Mesh*> meshes;
            for (size_t i = 0; i < scene->mNumMeshes; ++i)
            {
                aiMesh* mesh = scene->mMeshes[i];

                std::vector<Vertex> vertices;
                std::vector<GLuint> indices;

                for (size_t v = 0; v < mesh->mNumVertices; ++v)
                {
                    aiVector3D& position = mesh->mVertices[v];
                    aiVector3D& normal = mesh->mNormals[v];

                    Vertex vertex;
                    vertex.position = glm::vec3(position.x, position.y, position.z);
                    vertex.normal = glm::vec3(normal.x, normal.y, normal.z);

                    if (mesh->mTextureCoords[0])
                    {
                        vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
                    }

                    vertices.push_back(vertex);
                }

                for (size_t v = 0; v < mesh->mNumFaces; ++v)
                {
                    aiFace& face = mesh->mFaces[v];
                    for (size_t f = 0; f < face.mNumIndices; ++f)
                    {
                        indices.push_back(face.mIndices[f]);
                    }
                }

                meshes.push_back(new Mesh(vertices, indices));
            }

            lua_newtable(L);
            for (size_t i = 0; i < meshes.size(); ++i)
            {
                lua_pushnumber(L, i + 1);
                pushValue(L, meshes[i]);
                lua_settable(L, -3);
            }

            return 1;
        }

        int saveWakeModel(lua_State* L)
        {
            const char* path = luaL_checkstring(L, 1);

            luaL_checktype(L, 2, LUA_TTABLE);
            std::vector<Mesh*> meshes;
            lua_pushnil(L);
            while (lua_next(L, 2) != 0)
            {
                luaL_argcheck(L, lua_isnumber(L, -2), 1, "expected number index");
                Mesh* mesh = luaW_checkmesh(L, -1);
                meshes.push_back(mesh);
                lua_pop(L, 1);
            }

            std::stringstream out;

            out.write("wmd1", 4);
            uint32_t tmp = (uint32_t) meshes.size();
            out.write((char*) &tmp, sizeof(tmp));
            for (auto* mesh : meshes)
            {
                auto& vertices = mesh->getVertices();
                tmp = vertices.size();
                out.write((char*) &tmp, sizeof(tmp));
                for (auto& vertex : vertices)
                {
                    float f = vertex.position.x;
                    out.write((char*) &f, sizeof(f));

                    f = vertex.position.y;
                    out.write((char*) &f, sizeof(f));

                    f = vertex.position.z;
                    out.write((char*) &f, sizeof(f));

                    f = vertex.normal.x;
                    out.write((char*) &f, sizeof(f));

                    f = vertex.normal.y;
                    out.write((char*) &f, sizeof(f));

                    f = vertex.normal.z;
                    out.write((char*) &f, sizeof(f));

                    f = vertex.texCoords.x;
                    out.write((char*) &f, sizeof(f));

                    f = vertex.texCoords.y;
                    out.write((char*) &f, sizeof(f));
                }

                auto& indices = mesh->getIndices();
                tmp = indices.size();
                out.write((char*) &tmp, sizeof(tmp));
                for (auto index : indices)
                {
                    tmp = index;
                    out.write((char*) &tmp, sizeof(tmp));
                }
            }

            std::string source = out.str();
            std::string result;
            snappy::Compress(source.c_str(), source.length(), &result);

            std::fstream f;
            f.open(path, std::ios::out | std::ios::ate | std::ios::binary);
            if (!f.is_open())
            {
                std::cout << "saveWakeModel error: unable to open " << path << " for writing" << std::endl;
                lua_pushboolean(L, false);
                return 1;
            }

            f.write(result.c_str(), result.length());
            f.close();

            lua_pushboolean(L, true);
            return 1;
        }

        int loadWakeModel(lua_State* L)
        {
            const char* path = luaL_checkstring(L, 1);

            std::fstream f;
            f.open(path, std::ios::in | std::ios::binary);
            if (!f.is_open())
            {
                std::cout << "loadWakeModel error: unable to open " << path << " for reading" << std::endl;
                lua_pushnil(L);
                return 1;
            }

            std::string inStr((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
            f.close();

            std::string result;
            snappy::Uncompress(inStr.c_str(), inStr.length(), &result);

            std::stringstream in;
            in << result;

            std::vector<Mesh*> meshes;
            char header[4];

            in.read(header, 4);
            if (strcmp(header, "wmd1") != 0)
            {
                std::cout << "loadWakeModel error: invalid header, expected wmd1 but got " << header << std::endl;
                lua_pushnil(L);
                return 1;
            }

            uint32_t meshCount;
            in.read((char*) &meshCount, sizeof(meshCount));
            meshes.reserve(meshCount);
            for (size_t m = 0; m < meshCount; ++m)
            {
                std::vector<Vertex> vertices;
                uint32_t vertexCount;
                in.read((char*) &vertexCount, sizeof(vertexCount));
                vertices.reserve(vertexCount);
                for (size_t v = 0; v < vertexCount; ++v)
                {
                    Vertex vertex;
                    float f;

                    in.read((char*) &f, sizeof(f));
                    vertex.position.x = f;

                    in.read((char*) &f, sizeof(f));
                    vertex.position.y = f;

                    in.read((char*) &f, sizeof(f));
                    vertex.position.z = f;

                    in.read((char*) &f, sizeof(f));
                    vertex.normal.x = f;

                    in.read((char*) &f, sizeof(f));
                    vertex.normal.y = f;

                    in.read((char*) &f, sizeof(f));
                    vertex.normal.z = f;

                    in.read((char*) &f, sizeof(f));
                    vertex.texCoords.x = f;

                    in.read((char*) &f, sizeof(f));
                    vertex.texCoords.y = f;

                    vertices.push_back(vertex);
                }

                std::vector<GLuint> indices;
                uint32_t indexCount;
                in.read((char*) &indexCount, sizeof(indexCount));
                indices.reserve(indexCount);
                for (size_t i = 0; i < indexCount; ++i)
                {
                    uint32_t val;
                    in.read((char*) &val, sizeof(val));
                    indices.push_back(val);
                }

                Mesh* mesh = new Mesh(vertices, indices);
                meshes.push_back(mesh);
            }

            lua_newtable(L);
            for (size_t i = 0; i < meshes.size(); ++i)
            {
                lua_pushnumber(L, i + 1);
                pushValue(L, meshes[i]);
                lua_settable(L, -3);
            }

            return 1;
        }

        static const struct luaL_reg assetslib_f[] = {
                {"loadModel",     loadModel},
                {"saveWakeModel", saveWakeModel},
                {"loadWakeModel", loadWakeModel},
                {NULL, NULL}
        };

        int luaopen_assets(lua_State* L)
        {
            luaL_register(L, "assets", assetslib_f);
            return 1;
        }

        W_REGISTER_MODULE(luaopen_assets);
    }
}