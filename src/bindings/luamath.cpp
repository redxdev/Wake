#include "bindings/luamath.h"
#include "bindings/luamatrix.h"
#include "moduleregistry.h"

#include <glm/gtc/matrix_transform.hpp>

namespace wake
{
    namespace binding
    {
        int perspective(lua_State* L)
        {
            lua_Number fovy = luaL_checknumber(L, 1);
            lua_Number aspect = luaL_checknumber(L, 2);
            lua_Number near = luaL_checknumber(L, 3);
            lua_Number far = luaL_checknumber(L, 4);

            glm::mat4 result = glm::perspective(fovy, aspect, near, far);
            pushValue(L, result);
            return 1;
        }

        static const struct luaL_reg mathlib_f[] = {
                {"perspective", perspective},
                {NULL, NULL}
        };

        int luaopen_math(lua_State* L)
        {
            luaL_register(L, "math", mathlib_f);
            return 1;
        }

        W_REGISTER_MODULE(luaopen_math);
    }
}