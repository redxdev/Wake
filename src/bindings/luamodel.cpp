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
            if (lua_gettop(L) == 0)
            {
                pushValue(L, ModelPtr(new Model()));
            }
            else
            {
                luaL_checktype(L, 1, LUA_TTABLE);
                std::vector<ModelComponent> components;
                lua_pushnil(L);
                while (lua_next(L, 1) != 0)
                {
                    luaL_checktype(L, -1, LUA_TTABLE);
                    lua_pushstring(L, "mesh");
                    lua_gettable(L, -2);

                    MeshPtr mesh = luaW_checkmesh(L, -1);
                    lua_pop(L, 1);

                    lua_pushstring(L, "material");
                    lua_gettable(L, -2);

                    MaterialPtr material = luaW_checkmaterial(L, -1);
                    lua_pop(L, 1);

                    ModelComponent component;
                    component.mesh = mesh;
                    component.material = material;
                    components.push_back(component);
                }

                pushValue(L, ModelPtr(new Model(components)));
            }

            return 1;
        }

        static int setComponents(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            luaL_checktype(L, 2, LUA_TTABLE);

            std::vector<ModelComponent> components;
            lua_pushnil(L);
            while (lua_next(L, 1) != 0)
            {
                luaL_checktype(L, -1, LUA_TTABLE);
                lua_pushstring(L, "mesh");
                lua_gettable(L, -2);

                MeshPtr mesh = luaW_checkmesh(L, -1);
                lua_pop(L, 1);

                lua_pushstring(L, "material");
                lua_gettable(L, -2);

                MaterialPtr material = luaW_checkmaterial(L, -1);
                lua_pop(L, 1);

                ModelComponent component;
                component.mesh = mesh;
                component.material = material;
                components.push_back(component);
            }

            model->setComponents(components);
            return 0;
        }

        static int getComponents(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            auto& components = model->getComponents();

            lua_newtable(L);
            for (size_t i = 0; i < components.size(); ++i)
            {
                auto& component = components[i];

                lua_pushnumber(L, i + 1);
                lua_newtable(L);

                lua_pushstring(L, "mesh");
                pushValue(L, component.mesh);
                lua_settable(L, -3);

                lua_pushstring(L, "material");
                pushValue(L, component.material);
                lua_settable(L, -3);

                lua_settable(L, -3);
            }

            return 1;
        }

        static int getComponentCount(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            lua_pushinteger(L, (lua_Integer) model->getComponentCount());
            return 1;
        }

        static int getComponent(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            lua_Integer index = luaL_checkinteger(L, 2);
            luaL_argcheck(L, index > 0 && (size_t) index <= model->getComponentCount(), 2, "index out of range");

            auto& component = model->getComponent((size_t) index - 1);

            pushValue(L, component.mesh);
            pushValue(L, component.material);

            return 2;
        }

        static int setComponent(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            lua_Integer index = luaL_checkinteger(L, 2);
            luaL_argcheck(L, index > 0 && (size_t) index <= model->getComponentCount(), 2, "index out of range");
            MeshPtr mesh = luaW_checkmesh(L, 3);
            MaterialPtr material = luaW_checkmaterial(L, 4);

            ModelComponent component;
            component.mesh = mesh;
            component.material = material;

            model->setComponent((size_t) index - 1, component);

            return 0;
        }

        static int setMesh(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            lua_Integer index = luaL_checkinteger(L, 2);
            luaL_argcheck(L, index > 0 && (size_t) index <= model->getComponentCount(), 2, "index out of range");
            MeshPtr mesh = luaW_checkmesh(L, 3);

            model->setMesh((size_t) index - 1, mesh);

            return 0;
        }

        static int setMaterial(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            lua_Integer index = luaL_checkinteger(L, 2);
            luaL_argcheck(L, index > 0 && (size_t) index <= model->getComponentCount(), 2, "index out of range");
            MaterialPtr material = luaW_checkmaterial(L, 3);

            model->setMaterial((size_t) index - 1, material);

            return 0;
        }

        static int removeComponent(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            lua_Integer index = luaL_checkinteger(L, 2);
            luaL_argcheck(L, index > 0 && (size_t) index <= model->getComponentCount(), 2, "index out of range");

            model->removeComponent((size_t) index - 1);

            return 0;
        }

        static int addComponent(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            MeshPtr mesh = luaW_checkmesh(L, 2);
            MaterialPtr material = luaW_checkmaterial(L, 3);

            ModelComponent component;
            component.mesh = mesh;
            component.material = material;

            model->addComponent(component);

            return 0;
        }

        static int draw(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            model->draw();
            return 0;
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

        static int m_tostring(lua_State* L)
        {
            ModelPtr model = luaW_checkmodel(L, 1);
            std::stringstream ss;
            ss << "Model(" << model->getComponentCount() << ")";
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
                {"new",               model_new},
                {"setComponents",     setComponents},
                {"getComponents",     getComponents},
                {"getComponentCount", getComponentCount},
                {"getComponent",      getComponent},
                {"setComponent",      setComponent},
                {"setMesh",           setMesh},
                {"setMaterial",       setMaterial},
                {"removeComponent",   removeComponent},
                {"addComponent",      addComponent},
                {"draw",              draw},
                {"getMetadata",       getMetadata},
                {NULL, NULL}
        };

        static const struct luaL_reg modellib_m[] = {
                {"setComponents",     setComponents},
                {"getComponents",     getComponents},
                {"getComponentCount", getComponentCount},
                {"getComponent",      getComponent},
                {"setComponent",      setComponent},
                {"setMesh",           setMesh},
                {"setMaterial",       setMaterial},
                {"removeComponent",   removeComponent},
                {"addComponent",      addComponent},
                {"draw",              draw},
                {"getMetadata",       getMetadata},
                {"__tostring",        m_tostring},
                {"__gc",              m_gc},
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