#include "bindings/luaassets.h"
#include "bindings/luatexture.h"
#include "bindings/luamodel.h"
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

            ModelPtr model(new Model());
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

                ModelComponent component;
                component.mesh = MeshPtr(new Mesh(vertices, indices));
                model->addComponent(component);
            }

            ModelMetadata metadata;
            metadata.source = ModelMetadata::Assimp;
            metadata.path = path;

            model->setMetadata(metadata);

            pushValue(L, model);

            return 1;
        }

        static int loadWakeModel(lua_State* L, const char* path)
        {
            ModelPtr model = loadWMDL(path);
            pushValue(L, model);
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
            ModelPtr model = luaW_checkmodel(L, 2);

            bool compress = true;
            if (lua_gettop(L) >= 3)
            {
                compress = lua_toboolean(L, 3) != 0;
            }

            lua_pushboolean(L, saveWMDL(path, model, compress) ? 1 : 0);

            return 1;
        }

        static int loadTexture(lua_State* L)
        {
            const char* path = luaL_checkstring(L, 1);
            TexturePtr texture = Texture::load(path);
            if (texture == nullptr)
            {
                lua_pushnil(L);
                return 1;
            }

            pushValue(L, texture);
            return 1;
        }

        static const struct luaL_reg assetslib_f[] = {
                {"loadModel",   loadModel},
                {"saveModel",   saveModel},
                {"loadTexture", loadTexture},
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