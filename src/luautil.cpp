#include "luautil.h"

namespace wake
{
    namespace binding
    {
        bool checkMetatable(lua_State* L, int idx, const char* name)
        {
            if (lua_getmetatable(L, idx) == 0)
                return false;

            luaL_getmetatable(L, name);
            bool result = lua_equal(L, -1, -2) != 0;
            lua_pop(L, 2);
            return result;
        }
    }
}