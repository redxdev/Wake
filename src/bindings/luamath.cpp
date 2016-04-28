#include "bindings/luamath.h"
#include "bindings/luamatrix.h"
#include "moduleregistry.h"

#include <glm/gtc/matrix_transform.hpp>

namespace wake
{
    namespace binding
    {
        int frustum(lua_State* L)
        {
            lua_Number left = luaL_checknumber(L, 1);
            lua_Number right = luaL_checknumber(L, 2);
            lua_Number bottom = luaL_checknumber(L, 3);
            lua_Number top = luaL_checknumber(L, 4);
            lua_Number near = luaL_checknumber(L, 5);
            lua_Number far = luaL_checknumber(L, 6);

            glm::mat4 result = glm::frustum(left, right, bottom, top, near, far);
            pushValue(L, result);
            return 1;
        }

        int ortho(lua_State* L)
        {
            lua_Number left = luaL_checknumber(L, 1);
            lua_Number right = luaL_checknumber(L, 2);
            lua_Number bottom = luaL_checknumber(L, 3);
            lua_Number top = luaL_checknumber(L, 4);

            if (lua_gettop(L) >= 6)
            {
                lua_Number zNear = luaL_checknumber(L, 5);
                lua_Number zFar = luaL_checknumber(L, 6);

                glm::mat4 result = glm::ortho(left, right, bottom, top, zNear, zFar);
                pushValue(L, result);
            }
            else
            {
                glm::mat4 result = glm::ortho(left, right, bottom, top);
                pushValue(L, result);
            }

            return 1;
        }

        int perspective(lua_State* L)
        {
            lua_Number fovy = luaL_checknumber(L, 1);
            lua_Number aspect = luaL_checknumber(L, 2);
            lua_Number near = luaL_checknumber(L, 3);

            if (lua_gettop(L) >= 4)
            {
                lua_Number far = luaL_checknumber(L, 4);
                glm::mat4 result = glm::perspective(fovy, aspect, near, far);
                pushValue(L, result);
            }
            else
            {
                glm::mat4 result = glm::infinitePerspective(fovy, aspect, near);
                pushValue(L, result);
            }

            return 1;
        }

        int lookAt(lua_State* L)
        {
            glm::vec3& eye = *luaW_checkvector3(L, 1);
            glm::vec3& center = *luaW_checkvector3(L, 2);
            glm::vec3& up = *luaW_checkvector3(L, 3);

            glm::mat4 result = glm::lookAt(eye, center, up);
            pushValue(L, result);
            return 1;
        }

        static const struct luaL_reg mathlib_f[] = {
                {"frustum",     frustum},
                {"ortho",       ortho},
                {"perspective", perspective},
                {"lookAt",      lookAt},
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