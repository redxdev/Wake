#include "bindings/luaengine.h"
#include "bindings/luaevent.h"
#include "moduleregistry.h"

namespace wake
{
    namespace binding
    {
        static int tick(lua_State* L)
        {
            pushValue(L, W_ENGINE.TickEvent);
            return 1;
        }

        static const struct luaL_reg wakelib_f[] = {
                {"tick",	tick},
                {NULL, NULL}
        };

        int luaopen_engine(lua_State* L)
        {
            luaL_register(L, "engine", wakelib_f);
            return 1;
        }

        W_REGISTER_MODULE(luaopen_engine);
    }
}