#include "bindings/luaassets.h"
#include "bindings/luamesh.h"
#include "moduleregistry.h"
#include "wmdl.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

namespace wake
{
    namespace bindings
    {
        //
        // Model loading is currently implemented here until we have an actual Model class.
        // Eventually it will be moved to a static method on the model class so it can be
        // easily accessed from C++ as well as Lua.
        //

        static int loadAssimpModel(lua_State* L, const char* path)
        {
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

        static int loadWakeModel(lua_State* L, const char* path)
        {
            std::vector<Mesh*> meshes;
            bool result = loadWMDL(path, meshes);
            if (!result)
            {
                lua_pushnil(L);
                return 1;
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

        static int loadModel(lua_State* L)
        {
            std::string path(luaL_checkstring(L, 1));
            std::string::size_type pathPos = path.rfind('.');
            if (pathPos != std::string::npos)
            {
                std::string extension = path.substr(pathPos + 1);
                if (extension == "wmdl")
                {
                    return loadWakeModel(L, path.c_str());
                }
            }

            return loadAssimpModel(L, path.c_str());
        }

        static int saveModel(lua_State* L)
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


            bool compress = true;
            if (lua_gettop(L) >= 3)
            {
                compress = lua_toboolean(L, 3) != 0;
            }

            lua_pushboolean(L, saveWMDL(path, meshes, compress) ? 1 : 0);

            return 1;
        }

        static const struct luaL_reg assetslib_f[] = {
                {"loadModel", loadModel},
                {"saveModel", saveModel},
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