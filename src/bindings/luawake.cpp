#include "bindings/luawake.h"
#include "moduleregistry.h"

namespace wake
{
    namespace binding
    {
        static int getVersion(lua_State* L)
        {
            pushValue(L, wake::getVersion());
            return 1;
        }

        static int getEngineMode(lua_State* L)
        {
            switch (wake::getEngineMode())
            {
                case EngineMode::Invalid:
                    pushValue(L, "invalid");
                    break;

                case EngineMode::Normal:
                    pushValue(L, "normal");
                    break;

                case EngineMode::Testing:
                    pushValue(L, "testing");
                    break;

                case EngineMode::Tool:
                    pushValue(L, "tool");
                    break;
            }

            return 1;
        }

        static int getArguments(lua_State* L)
        {
            lua_newtable(L);
            for (size_t i = 0; i < getEngineArguments().size(); ++i)
            {
                lua_pushnumber(L, i + 1);
                pushValue(L, getEngineArguments()[i]);
                lua_settable(L, -3);
            }

            return 1;
        }

        static const struct luaL_reg wakelib_f[] = {
                {"getVersion",    getVersion},
                {"getEngineMode", getEngineMode},
                {"getArguments",  getArguments},
                {NULL, NULL}
        };

        int luaopen_wake(lua_State* L)
        {
            luaL_register(L, "wake", wakelib_f);
            return 1;
        }

        W_REGISTER_MODULE(luaopen_wake);
    }
}