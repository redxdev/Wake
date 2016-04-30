#include "bindings/luaassets.h"
#include "bindings/luamesh.h"
#include "moduleregistry.h"

#include <tiny_obj_loader.h>
#include <iostream>

namespace wake
{
    namespace bindings
    {
        int loadOBJ(lua_State* L)
        {
            const char* path = luaL_checkstring(L, 1);

            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string err;

            if (!tinyobj::LoadObj(shapes, materials, err, path))
            {
                std::cout << "loadOBJ error: " << err << std::endl;
                lua_pushnil(L);
                return 1;
            }

            if (!err.empty())
            {
                std::cout << "loadOBJ: " << err << std::endl;
            }

            std::vector<Mesh*> meshes;

            for (auto shape : shapes)
            {
                std::vector<Vertex> vertices;
                std::vector<GLuint> indices;

                size_t vertexCount = shape.mesh.positions.size() / 3;
                for (size_t v = 0; v < vertexCount; ++v)
                {
                    glm::vec3 position(shape.mesh.positions[v * 3], shape.mesh.positions[v * 3 + 1],
                                       shape.mesh.positions[v * 3 + 2]);

                    glm::vec3 normal(1, 0, 0);
                    if ((v * 3 + 2) < shape.mesh.normals.size())
                    {
                        normal = -glm::vec3(shape.mesh.normals[v * 3], shape.mesh.normals[v * 3 + 1],
                                           shape.mesh.normals[v * 3 + 2]);
                    }

                    glm::vec2 texCoords(0, 0);
                    if ((v * 2 + 1) < shape.mesh.texcoords.size())
                    {
                        texCoords = glm::vec2(shape.mesh.texcoords[v * 2], shape.mesh.texcoords[v * 2 + 1]);
                    }

                    Vertex vertex(position, normal, texCoords);
                    vertices.push_back(vertex);
                }

                for (auto index : shape.mesh.indices)
                {
                    indices.push_back((GLuint) index);
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
                {"loadOBJ", loadOBJ},
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