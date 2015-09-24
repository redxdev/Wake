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
                case EngineMode::INVALID:
                    pushValue(L, "invalid");
                    break;

                case EngineMode::NORMAL:
                    pushValue(L, "normal");
                    break;

                case EngineMode::TESTING:
                    pushValue(L, "testing");
                    break;
            }

            return 1;
        }

        static const struct luaL_reg wakelib_f[] = {
                {"getVersion",    getVersion},
                {"getEngineMode", getEngineMode},
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