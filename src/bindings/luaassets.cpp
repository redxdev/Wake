#include "bindings/luaassets.h"
#include "bindings/luamesh.h"
#include "moduleregistry.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

namespace wake
{
    namespace bindings
    {
        int loadModel(lua_State* L)
        {
            const char* path = luaL_checkstring(L, 1);

            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

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

        static const struct luaL_reg assetslib_f[] = {
                {"loadModel", loadModel},
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