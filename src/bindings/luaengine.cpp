#include "bindings/luaengine.h"
#include "bindings/luaevent.h"
#include "moduleregistry.h"

namespace wake
{
    namespace binding
    {
        static int stop(lua_State* L)
        {
            W_ENGINE.stop();
            return 1;
        }

        static const struct luaL_reg wakelib_f[] = {
                {"stop",    stop},
                {NULL, NULL}
        };

        int luaopen_engine(lua_State* L)
        {
            luaL_register(L, "engine", wakelib_f);

            lua_pushstring(L, "tick");
            pushValue(L, W_ENGINE.TickEvent);
            lua_settable(L, -3);

            lua_pushstring(L, "quit");
            pushValue(L, W_ENGINE.QuitEvent);
            lua_settable(L, -3);

            return 1;
        }

        W_REGISTER_MODULE(luaopen_engine);
    }
}