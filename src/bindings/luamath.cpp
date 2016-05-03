#include "bindings/luamath.h"
#include "bindings/luamatrix.h"
#include "moduleregistry.h"

#include <glm/gtx/transform.hpp>

namespace wake
{
    namespace binding
    {
        static int degrees(lua_State* L)
        {
            lua_Number rad = luaL_checknumber(L, 1);
            lua_Number deg = glm::degrees(rad);
            pushValue(L, deg);
            return 1;
        }

        static int radians(lua_State* L)
        {
            lua_Number deg = luaL_checknumber(L, 1);
            lua_Number rad = glm::radians(deg);
            pushValue(L, rad);
            return 1;
        }

        static int frustum(lua_State* L)
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

        static int lookAt(lua_State* L)
        {
            glm::vec3& eye = *luaW_checkvector3(L, 1);
            glm::vec3& center = *luaW_checkvector3(L, 2);
            glm::vec3& up = *luaW_checkvector3(L, 3);

            glm::mat4 result = glm::lookAt(eye, center, up);
            pushValue(L, result);
            return 1;
        }

        static int ortho(lua_State* L)
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

        static int perspective(lua_State* L)
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

        static int rotate(lua_State* L)
        {
            int argCount = lua_gettop(L);
            if (argCount <= 2)
            {
                lua_Number angle = luaL_checknumber(L, 1);
                glm::vec3& v = *luaW_checkvector3(L, 2);

                glm::mat4 result = glm::rotate((float) angle, v);
                pushValue(L, result);
            }
            else
            {
                glm::mat4& m = *luaW_checkmatrix4x4(L, 1);
                lua_Number angle = luaL_checknumber(L, 2);
                glm::vec3& v = *luaW_checkvector3(L, 3);

                glm::mat4 result = glm::rotate(m, (float) angle, v);
                pushValue(L, result);
            }

            return 1;
        }

        static int scale(lua_State* L)
        {
            int argCount = lua_gettop(L);
            if (argCount <= 1)
            {
                glm::vec3& v = *luaW_checkvector3(L, 1);

                glm::mat4 result = glm::scale(v);
                pushValue(L, result);
            }
            else
            {
                glm::mat4& m = *luaW_checkmatrix4x4(L, 1);
                glm::vec3& v = *luaW_checkvector3(L, 2);

                glm::mat4 result = glm::scale(m, v);
                pushValue(L, result);
            }

            return 1;
        }

        static int translate(lua_State* L)
        {
            int argCount = lua_gettop(L);
            if (argCount <= 1)
            {
                glm::vec3& v = *luaW_checkvector3(L, 1);

                glm::mat4 result = glm::translate(v);
                pushValue(L, result);
            }
            else
            {
                glm::mat4& m = *luaW_checkmatrix4x4(L, 1);
                glm::vec3& v = *luaW_checkvector3(L, 2);

                glm::mat4 result = glm::translate(m, v);
                pushValue(L, result);
            }

            return 1;
        }

        static int abs(lua_State* L)
        {
            if (lua_isnumber(L, 1))
            {
                lua_Number x = luaL_checknumber(L, 1);
                pushValue(L, glm::abs(x));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec2>::metatable()))
            {
                auto& x = *luaW_checkvector2(L, 1);
                pushValue(L, glm::abs(x));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec3>::metatable()))
            {
                auto& x = *luaW_checkvector3(L, 1);
                pushValue(L, glm::abs(x));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec4>::metatable()))
            {
                auto& x = *luaW_checkvector4(L, 1);
                pushValue(L, glm::abs(x));
                return 1;
            }

            luaL_error(L, "Unknown type passed to math.abs");
            return 0;
        }

        static int ceil(lua_State* L)
        {
            if (lua_isnumber(L, 1))
            {
                lua_Number x = luaL_checknumber(L, 1);
                pushValue(L, glm::ceil(x));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec2>::metatable()))
            {
                auto& x = *luaW_checkvector2(L, 1);
                pushValue(L, glm::ceil(x));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec3>::metatable()))
            {
                auto& x = *luaW_checkvector3(L, 1);
                pushValue(L, glm::ceil(x));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec4>::metatable()))
            {
                auto& x = *luaW_checkvector4(L, 1);
                pushValue(L, glm::ceil(x));
                return 1;
            }

            luaL_error(L, "Unknown type passed to math.ceil");
            return 0;
        }

        static int clamp(lua_State* L)
        {
            if (lua_isnumber(L, 1))
            {
                lua_Number x = luaL_checknumber(L, 1);
                lua_Number min = luaL_checknumber(L, 2);
                lua_Number max = luaL_checknumber(L, 3);
                pushValue(L, glm::clamp(x, min, max));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec2>::metatable()))
            {
                auto& x = *luaW_checkvector2(L, 1);
                auto& min = *luaW_checkvector2(L, 2);
                auto& max = *luaW_checkvector2(L, 3);
                pushValue(L, glm::clamp(x, min, max));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec3>::metatable()))
            {
                auto& x = *luaW_checkvector3(L, 1);
                auto& min = *luaW_checkvector3(L, 2);
                auto& max = *luaW_checkvector3(L, 3);
                pushValue(L, glm::clamp(x, min, max));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec4>::metatable()))
            {
                auto& x = *luaW_checkvector4(L, 1);
                auto& min = *luaW_checkvector4(L, 2);
                auto& max = *luaW_checkvector4(L, 3);
                pushValue(L, glm::clamp(x, min, max));
                return 1;
            }

            luaL_error(L, "Unknown type passed to math.clamp");
            return 0;
        }

        static int floor(lua_State* L)
        {
            if (lua_isnumber(L, 1))
            {
                lua_Number x = luaL_checknumber(L, 1);
                pushValue(L, glm::floor(x));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec2>::metatable()))
            {
                auto& x = *luaW_checkvector2(L, 1);
                pushValue(L, glm::floor(x));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec3>::metatable()))
            {
                auto& x = *luaW_checkvector3(L, 1);
                pushValue(L, glm::floor(x));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec4>::metatable()))
            {
                auto& x = *luaW_checkvector4(L, 1);
                pushValue(L, glm::floor(x));
                return 1;
            }

            luaL_error(L, "Unknown type passed to math.floor");
            return 0;
        }

        static int fract(lua_State* L)
        {
            if (lua_isnumber(L, 1))
            {
                lua_Number x = luaL_checknumber(L, 1);
                pushValue(L, glm::fract(x));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec2>::metatable()))
            {
                auto& x = *luaW_checkvector2(L, 1);
                pushValue(L, glm::fract(x));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec3>::metatable()))
            {
                auto& x = *luaW_checkvector3(L, 1);
                pushValue(L, glm::fract(x));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec4>::metatable()))
            {
                auto& x = *luaW_checkvector4(L, 1);
                pushValue(L, glm::fract(x));
                return 1;
            }

            luaL_error(L, "Unknown type passed to math.fract");
            return 0;
        }

        static int round(lua_State* L)
        {
            if (lua_isnumber(L, 1))
            {
                lua_Number x = luaL_checknumber(L, 1);
                pushValue(L, glm::round(x));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec2>::metatable()))
            {
                auto& x = *luaW_checkvector2(L, 1);
                pushValue(L, glm::round(x));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec3>::metatable()))
            {
                auto& x = *luaW_checkvector3(L, 1);
                pushValue(L, glm::round(x));
                return 1;
            }
            else if (checkMetatable(L, 1, VectorInfo<glm::vec4>::metatable()))
            {
                auto& x = *luaW_checkvector4(L, 1);
                pushValue(L, glm::round(x));
                return 1;
            }

            luaL_error(L, "Unknown type passed to math.round");
            return 0;
        }

        static const struct luaL_reg mathlib_f[] = {
                {"degrees",     degrees},
                {"radians",     radians},
                {"frustum",     frustum},
                {"lookAt",      lookAt},
                {"ortho",       ortho},
                {"perspective", perspective},
                {"rotate",      rotate},
                {"scale",       scale},
                {"translate",   translate},
                {"abs",         abs},
                {"ceil",        ceil},
                {"clamp",       clamp},
                {"floor",       floor},
                {"fract",       fract},
                {"round",       round},
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