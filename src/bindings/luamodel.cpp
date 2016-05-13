#include "bindings/luamodel.h"
#include "bindings/luamesh.h"
#include "bindings/luamaterial.h"
#include "moduleregistry.h"

#include <sstream>
#include <cstring>

namespace wake
{
    namespace binding
    {
        struct ModelContainer
        {
            ModelPtr model;
        };

        static int model_new(lua_State* L)
        {
            pushValue(L, ModelPtr(new Model()));
            return 1;
        }

        static int getMetadata(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            auto& metadata = model->getMetadata();

            lua_newtable(L);

            lua_pushstring(L, "source");

            switch (metadata.source)
            {
                default:
                    lua_pushstring(L, "unknown");
                    break;

                case ModelMetadata::WMDL:
                    lua_pushstring(L, "wmdl");
                    break;

                case ModelMetadata::Assimp:
                    lua_pushstring(L, "assimp");
                    break;
            }

            lua_settable(L, -3);

            lua_pushstring(L, "version");
            lua_pushnumber(L, (lua_Number) metadata.version);
            lua_settable(L, -3);

            lua_pushstring(L, "path");
            lua_pushstring(L, metadata.path.data());
            lua_settable(L, -3);

            return 1;
        }

        static int getMaterialCount(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            lua_pushinteger(L, (lua_Integer) model->getMaterialCount());
            return 1;
        }

        static int getMaterials(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            auto& materials = model->getMaterials();

            lua_newtable(L);
            for (size_t i = 0; i < materials.size(); ++i)
            {
                auto& matInfo = materials[i];

                lua_pushnumber(L, i + 1);
                lua_newtable(L);

                lua_pushstring(L, "material");
                pushValue(L, matInfo.material);
                lua_settable(L, -3);

                lua_pushstring(L, "name");
                lua_pushstring(L, matInfo.name.data());
                lua_settable(L, -3);

                lua_settable(L, -3);
            }

            return 1;
        }

        static int getMaterial(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            int32 index = (int32) luaL_checkinteger(L, 2) - 1;

            auto& matInfo = model->getMaterial(index);
            if (matInfo.material.get() == nullptr)
            {
                lua_pushnil(L);
                return 1;
            }

            lua_newtable(L);

            lua_pushstring(L, "material");
            pushValue(L, matInfo.material);
            lua_settable(L, -3);

            lua_pushstring(L, "name");
            lua_pushstring(L, matInfo.name.data());
            lua_settable(L, -3);

            return 1;
        }

        static int getMaterialByName(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            const char* name = luaL_checkstring(L, 2);

            auto& matInfo = model->getMaterialByName(name);
            if (matInfo.material.get() == nullptr)
            {
                lua_pushnil(L);
                return 1;
            }

            lua_newtable(L);

            lua_pushstring(L, "material");
            pushValue(L, matInfo.material);
            lua_settable(L, -3);

            lua_pushstring(L, "name");
            lua_pushstring(L, matInfo.name.data());
            lua_settable(L, -3);

            return 1;
        }

        static int getMaterialIndex(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            const char* name = luaL_checkstring(L, 2);

            lua_pushinteger(L, (lua_Integer) model->getMaterialIndex(name) + 1);
            return 1;
        }

        static int setMaterial(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            int32 index = (int32) luaL_checkinteger(L, 2) - 1;
            MaterialPtr mat = luaW_checkmaterial(L, 3);

            lua_pushboolean(L, model->setMaterial(index, mat) ? 1 : 0);
            return 1;
        }

        static int setMaterialByName(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            const char* name = luaL_checkstring(L, 2);
            MaterialPtr mat = luaW_checkmaterial(L, 3);

            lua_pushboolean(L, model->setMaterialByName(name, mat) ? 1 : 0);
            return 1;
        }

        static int addMaterial(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            const char* name = luaL_checkstring(L, 2);
            MaterialPtr mat = luaW_checkmaterial(L, 3);

            lua_pushboolean(L, model->addMaterial(name, mat) ? 1 : 0);
            return 1;
        }

        static int hasMaterialName(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            const char* name = luaL_checkstring(L, 2);

            lua_pushboolean(L, model->hasMaterialName(name) ? 1 : 0);
            return 1;
        }

        static int renameMaterial(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            int32 index = (int32) luaL_checkinteger(L, 2) - 1;
            const char* newName = luaL_checkstring(L, 3);

            lua_pushboolean(L, model->renameMaterial(index, newName) ? 1 : 0);
            return 1;
        }

        static int removeMaterial(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            int32 index = (int32) luaL_checkinteger(L, 2) - 1;

            lua_pushboolean(L, model->removeMaterial(index) ? 1 : 0);
            return 1;
        }

        static int removeMaterialByName(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            const char* name = luaL_checkstring(L, 2);

            lua_pushboolean(L, model->removeMaterialByName(name) ? 1 : 0);
            return 1;
        }

        static int getMeshCount(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            lua_pushinteger(L, (lua_Integer) model->getMeshCount());
            return 1;
        }

        static int getMeshes(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);

            auto& meshes = model->getMeshes();

            lua_newtable(L);
            for (size_t i = 0; i < meshes.size(); ++i)
            {
                auto& meshInfo = meshes[i];

                lua_pushnumber(L, i + 1);
                lua_newtable(L);

                lua_pushstring(L, "mesh");
                pushValue(L, meshInfo.mesh);
                lua_settable(L, -3);

                lua_pushstring(L, "materialIndex");
                lua_pushinteger(L, (lua_Integer) meshInfo.materialIndex);
                lua_settable(L, -3);

                lua_settable(L, -3);
            }

            return 1;
        }

        static int getMesh(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            int32 index = (int32) luaL_checkinteger(L, 2) - 1;

            auto& meshInfo = model->getMesh(index);
            if (meshInfo.mesh.get() == nullptr)
            {
                lua_pushnil(L);
                return 1;
            }

            lua_newtable(L);

            lua_pushstring(L, "mesh");
            pushValue(L, meshInfo.mesh);
            lua_settable(L, -3);

            lua_pushstring(L, "materialIndex");
            lua_pushinteger(L, (lua_Integer) meshInfo.materialIndex);
            lua_settable(L, -3);

            return 1;
        }

        static int setMesh(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            int32 index = (int32) luaL_checkinteger(L, 2) - 1;
            MeshPtr mesh = luaW_checkmesh(L, 3);

            lua_pushboolean(L, model->setMesh(index, mesh) ? 1 : 0);
            return 1;
        }

        static int setMeshMaterial(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            int32 index = (int32) luaL_checkinteger(L, 2) - 1;
            int32 materialIndex = (int32) luaL_checkinteger(L, 3) - 1;

            lua_pushboolean(L, model->setMeshMaterial(index, materialIndex) ? 1 : 0);
            return 1;
        }

        static int setMeshMaterialByName(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            int32 index = (int32) luaL_checkinteger(L, 2) - 1;
            const char* name = luaL_checkstring(L, 1);

            lua_pushboolean(L, model->setMeshMaterialByName(index, name) ? 1 : 0);
            return 1;
        }

        static int addMesh(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            MeshPtr mesh = luaW_checkmesh(L, 2);
            int32 materialIndex = (int32) luaL_checkinteger(L, 3) - 1;

            model->addMesh(mesh, materialIndex);
            return 0;
        }

        static int removeMesh(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            int32 index = (int32) luaL_checkinteger(L, 2) - 1;

            lua_pushboolean(L, model->removeMesh(index) ? 1 : 0);
            return 1;
        }

        static int draw(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            model->draw();
            return 0;
        }

        static int m_tostring(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            std::stringstream ss;
            ss << "Model(" << model->getMeshCount() << ")";
            std::string str = ss.str();
            lua_pushstring(L, str.c_str());
            return 1;
        }

        static int m_gc(lua_State* L)
        {
            void* dataPtr = luaL_checkudata(L, 1, W_MT_MODEL);
            luaL_argcheck(L, dataPtr != nullptr, 1, "'Model' expected");
            ModelContainer* container = (ModelContainer*) dataPtr;
            container->model.reset();
            return 0;
        }

        static const struct luaL_reg modellib_f[] = {
                {"new",                   model_new},
                {"getMetadata",           getMetadata},
                {"getMaterialCount",      getMaterialCount},
                {"getMaterials",          getMaterials},
                {"getMaterial",           getMaterial},
                {"getMaterialByName",     getMaterialByName},
                {"getMaterialIndex",      getMaterialIndex},
                {"setMaterial",           setMaterial},
                {"setMaterialByName",     setMaterialByName},
                {"addMaterial",           addMaterial},
                {"hasMaterialName",       hasMaterialName},
                {"renameMaterial",        renameMaterial},
                {"removeMaterial",        removeMaterial},
                {"removeMaterialByName",  removeMaterialByName},
                {"getMeshCount",          getMeshCount},
                {"getMeshes",             getMeshes},
                {"getMesh",               getMesh},
                {"setMesh",               setMesh},
                {"setMeshMaterial",       setMeshMaterial},
                {"setMeshMaterialByName", setMeshMaterialByName},
                {"addMesh",               addMesh},
                {"removeMesh",            removeMesh},
                {"draw",                  draw},
                {NULL, NULL}
        };

        static const struct luaL_reg modellib_m[] = {
                {"getMetadata",           getMetadata},
                {"getMaterialCount",      getMaterialCount},
                {"getMaterials",          getMaterials},
                {"getMaterial",           getMaterial},
                {"getMaterialByName",     getMaterialByName},
                {"getMaterialIndex",      getMaterialIndex},
                {"setMaterial",           setMaterial},
                {"setMaterialByName",     setMaterialByName},
                {"addMaterial",           addMaterial},
                {"hasMaterialName",       hasMaterialName},
                {"renameMaterial",        renameMaterial},
                {"removeMaterial",        removeMaterial},
                {"removeMaterialByName",  removeMaterialByName},
                {"getMeshCount",          getMeshCount},
                {"getMeshes",             getMeshes},
                {"getMesh",               getMesh},
                {"setMesh",               setMesh},
                {"setMeshMaterial",       setMeshMaterial},
                {"setMeshMaterialByName", setMeshMaterialByName},
                {"addMesh",               addMesh},
                {"removeMesh",            removeMesh},
                {"draw",                  draw},
                {"__tostring",            m_tostring},
                {"__gc",                  m_gc},
                {NULL, NULL}
        };

        int luaopen_model(lua_State* L)
        {
            luaL_newmetatable(L, W_MT_MODEL);

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);

            luaL_register(L, NULL, modellib_m);

            luaL_register(L, "Model", modellib_f);
            return 1;
        }

        W_REGISTER_MODULE(luaopen_model);
    }

    void pushValue(lua_State* L, ModelPtr value)
    {
        if (value.get() == nullptr)
        {
            lua_pushnil(L);
            return;
        }

        auto* container = (binding::ModelContainer*) lua_newuserdata(L, sizeof(binding::ModelContainer));
        memset(container, 0, sizeof(binding::ModelContainer));
        container->model = value;
        luaL_getmetatable(L, W_MT_MODEL);
        lua_setmetatable(L, -2);
    }

    ModelPtr luaW_checkmodel(lua_State* L, int narg)
    {
        void* dataPtr = luaL_checkudata(L, narg, W_MT_MODEL);
        luaL_argcheck(L, dataPtr != nullptr, narg, "'Model' expected");
        binding::ModelContainer* container = (binding::ModelContainer*) dataPtr;
        return container->model;
    }
}