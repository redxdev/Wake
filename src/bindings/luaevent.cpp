#include "bindings/luaevent.h"
#include "moduleregistry.h"

namespace wake
{
    namespace binding
    {
        static int bind(lua_State* L)
        {
            auto wrapper = luaW_checkevent(L, 1);

            switch (lua_gettop(L))
            {
                default:
                    luaL_error(L, "bind requires 2 or 3 arguments");
                    break;

                case 2:
                    luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");
                    lua_pushvalue(L, 2);
                    wrapper->bind(L);
                    break;

                case 3:
                    luaL_argcheck(L, lua_istable(L, 2), 2, "'table' expected");
                    lua_pushvalue(L, 2);
                    luaL_argcheck(L, lua_isfunction(L, 3), 3, "'function' expected");
                    lua_pushvalue(L, 3);
                    wrapper->bind(L);
                    break;
            }

            return 0;
        }

        static int unbind(lua_State* L)
        {
            auto wrapper = luaW_checkevent(L, 1);

            switch (lua_gettop(L))
            {
                default:
                    luaL_error(L, "unbind requires 2 or 3 arguments");
                    break;

                case 2:
                    luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");
                    lua_pushvalue(L, 2);
                    wrapper->unbind(L);
                    break;

                case 3:
                    luaL_argcheck(L, lua_istable(L, 2), 2, "'table' expected");
                    lua_pushvalue(L, 2);
                    luaL_argcheck(L, lua_isfunction(L, 3), 3, "'function' expected");
                    lua_pushvalue(L, 3);
                    wrapper->unbind(L);
                    break;
            }

            return 0;
        }

        static int clear(lua_State* L)
        {
            auto wrapper = luaW_checkevent(L, 1);
            wrapper->clear();

            return 0;
        }

        static int m_gc(lua_State* L)
        {
            delete luaW_checkevent(L, 1);
            return 0;
        }

        static int m_tostring(lua_State* L)
        {
            luaW_checkevent(L, 1);
            lua_pushstring(L, "Event<...>");
            return 1;
        }

        static const struct luaL_reg eventlib_f[] = {
                {"bind",   bind},
                {"unbind", unbind},
                {"clear",  clear},
                {NULL, NULL}
        };

        static const struct luaL_reg eventlib_m[] = {
                {"__gc",       m_gc},
                {"__tostring", m_tostring},
                {"bind",       bind},
                {"unbind",     unbind},
                {"clear",      clear},
                {NULL, NULL}
        };

        int luaopen_event(lua_State* L)
        {
            luaL_newmetatable(L, W_MT_EVENT);

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);

            luaL_register(L, NULL, eventlib_m);

            luaL_register(L, "NativeEvent", eventlib_f);
            return 1;
        }

        W_REGISTER_MODULE_PRIORITY(luaopen_event, 100);
    }

    binding::ILuaEventWrapper* luaW_checkevent(lua_State* L, int narg)
    {
        void* dataPtr = luaL_checkudata(L, narg, W_MT_EVENT);
        luaL_argcheck(L, dataPtr != nullptr, narg, "'NativeEvent' expected");
        binding::EventContainer* container = (binding::EventContainer*) dataPtr;
        return container->wrapper;
    }
}