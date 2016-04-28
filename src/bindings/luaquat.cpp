#include "bindings/luaquat.h"
#include "bindings/luamatrix.h"
#include "moduleregistry.h"

#include <glm/gtx/quaternion.hpp>
#include <sstream>

namespace wake
{
    namespace binding
    {
        struct QuatContainer
        {
            glm::quat* quat;
        };

        static int quat_new(lua_State* L)
        {
            int argCount = lua_gettop(L);
            switch (argCount)
            {
                default:
                    luaL_error(L, "wrong number of arguments for Quat.new");
                    return 0;

                case 0:
                    pushValue(L, glm::quat());
                    return 1;

                case 1:
                {
                    if (checkMetatable(L, 1, W_MT_QUAT))
                    {
                        glm::quat* other = luaW_checkquat(L, 1);
                        pushValue(L, *other);
                        return 1;
                    }
                    else
                    {
                        glm::vec3& other = *luaW_checkvector3(L, 1);
                        pushValue(L, glm::quat(other));
                        return 1;
                    }
                }

                case 2:
                {
                    glm::vec3& a = *luaW_checkvector3(L, 1);
                    glm::vec3& b = *luaW_checkvector3(L, 2);

                    pushValue(L, glm::quat(a, b));
                    return 1;
                }

                case 4:
                {
                    lua_Number w = luaL_checknumber(L, 1);
                    lua_Number x = luaL_checknumber(L, 2);
                    lua_Number y = luaL_checknumber(L, 3);
                    lua_Number z = luaL_checknumber(L, 4);

                    pushValue(L, glm::quat(w, x, y, z));
                    return 1;
                }
            }
        }

        static int table(lua_State* L)
        {
            glm::quat& quat = *luaW_checkquat(L, 1);
            lua_newtable(L);
            for (int i = 0; i < 4; ++i)
            {
                lua_pushnumber(L, i + 1);
                lua_pushnumber(L, quat[i]);
                lua_settable(L, -3);
            }

            return 1;
        }

        static int get(lua_State* L)
        {
            glm::quat& quat = *luaW_checkquat(L, 1);
            int index = luaL_checkinteger(L, 2);
            luaL_argcheck(L, index >= 1 && index <= 4, 2, "index out of range");
            lua_pushnumber(L, quat[index - 1]);
            return 1;
        }

        static int set(lua_State* L)
        {
            glm::quat& quat = *luaW_checkquat(L, 1);
            int index = luaL_checkinteger(L, 2);
            luaL_argcheck(L, index >= 1 && index <= 4, 2, "index out of range");
            float value = (float) luaL_checknumber(L, 3);
            quat[index - 1] = value;
            return 0;
        }

        static int setAll(lua_State* L)
        {
            glm::quat& quat = *luaW_checkquat(L, 1);
            for (int i = 0; i < 4; ++i)
            {
                quat[i] = (float) luaL_checknumber(L, i + 2);
            }

            return 0;
        }

        static int conjugate(lua_State* L)
        {
            glm::quat& quat = *luaW_checkquat(L, 1);
            pushValue(L, glm::conjugate(quat));
            return 1;
        }

        static int cross(lua_State* L)
        {
            glm::quat& a = *luaW_checkquat(L, 1);
            glm::quat& b = *luaW_checkquat(L, 2);
            pushValue(L, glm::cross(a, b));
            return 1;
        }

        static int dot(lua_State* L)
        {
            glm::quat& a = *luaW_checkquat(L, 1);
            glm::quat& b = *luaW_checkquat(L, 2);
            pushValue(L, glm::dot(a, b));
            return 1;
        }

        static int inverse(lua_State* L)
        {
            glm::quat& quat = *luaW_checkquat(L, 1);
            pushValue(L, glm::inverse(quat));
            return 1;
        }

        static int length(lua_State* L)
        {
            glm::quat& quat = *luaW_checkquat(L, 1);
            pushValue(L, glm::length(quat));
            return 1;
        }

        static int apply(lua_State* L)
        {
            glm::quat& quat = *luaW_checkquat(L, 1);
            luaL_argcheck(L, lua_isfunction(L, 2), 2, "'function' expected");

            glm::quat result;
            for (int i = 0; i < 4; ++i)
            {
                lua_pushvalue(L, 2);
                lua_pushnumber(L, quat[i]);
                lua_pushnumber(L, i + 1);
                lua_call(L, 2, 1);

                result[i] = (float) lua_tonumber(L, -1);
                lua_pop(L, 1);
            }

            pushValue(L, result);
            return 1;
        }

        static int mat3(lua_State* L)
        {
            glm::quat& quat = *luaW_checkquat(L, 1);
            pushValue(L, glm::mat3_cast(quat));
            return 1;
        }

        static int mat4(lua_State* L)
        {
            glm::quat& quat = *luaW_checkquat(L, 1);
            pushValue(L, glm::mat4_cast(quat));
            return 1;
        }

        static int mix(lua_State* L)
        {
            glm::quat& a = *luaW_checkquat(L, 1);
            glm::quat& b = *luaW_checkquat(L, 2);
            lua_Number alpha = luaL_checknumber(L, 3);

            pushValue(L, glm::mix(a, b, (float) alpha));
            return 1;
        }

        static int normalize(lua_State* L)
        {
            glm::quat& quat = *luaW_checkquat(L, 1);
            pushValue(L, glm::normalize(quat));
            return 1;
        }

        static int rotate(lua_State* L)
        {
            glm::quat& quat = *luaW_checkquat(L, 1);
            lua_Number angle = luaL_checknumber(L, 2);
            glm::vec3& v = *luaW_checkvector3(L, 3);

            glm::quat() + glm::quat();

            pushValue(L, glm::rotate(quat, (float) angle, v));
            return 1;
        }

        static int m_add(lua_State* L)
        {
            glm::quat& a = *luaW_checkquat(L, 1);
            glm::quat& b = *luaW_checkquat(L, 2);
            pushValue(L, a + b);
            return 1;
        }

        static int m_mul(lua_State* L)
        {
            glm::quat& a = *luaW_checkquat(L, 1);

            if (checkMetatable(L, 2, W_MT_QUAT))
            {
                glm::quat& b = *luaW_checkquat(L, 2);
                pushValue(L, a * b);
                return 1;
            }
            else if (checkMetatable(L, 2, VectorInfo<glm::vec3>::metatable()))
            {
                glm::vec3& b = *luaW_checkvector3(L, 2);
                pushValue(L, a * b);
                return 1;
            }
            else if (checkMetatable(L, 2, VectorInfo<glm::vec4>::metatable()))
            {
                glm::vec4& b = *luaW_checkvector4(L, 2);
                pushValue(L, a * b);
                return 1;
            }

            luaL_error(L, "Cannot apply operation to incompatible types!");
            return 0;
        }

        static int m_gc(lua_State* L)
        {
            delete luaW_checkquat(L, 1);
            return 0;
        }

        static int m_tostring(lua_State* L)
        {
            glm::quat& quat = *luaW_checkquat(L, 1);
            std::stringstream ss;
            ss << "quat(" << quat[0] << "," << quat[1] << "," << quat[2] << "," << quat[3] << ")";
            auto str = ss.str();
            lua_pushstring(L, str.c_str());
            return 1;
        }

        static int m_eq(lua_State* L)
        {
            glm::quat& a = *luaW_checkquat(L, 1);
            glm::quat& b = *luaW_checkquat(L, 2);
            lua_pushboolean(L, a == b);
            return 1;
        }

        static int m_len(lua_State* L)
        {
            lua_pushnumber(L, 4);
            return 1;
        }

        static int m_unm(lua_State* L)
        {
            glm::quat& quat = *luaW_checkquat(L, 1);
            pushValue(L, -quat);
            return 1;
        }

        static const luaL_reg quatlib_f[] = {
                {"new",       quat_new},
                {"table",     table},
                {"get",       get},
                {"set",       set},
                {"setAll",    setAll},
                {"conjugate", conjugate},
                {"cross",     cross},
                {"dot",       dot},
                {"inverse",   inverse},
                {"length",    length},
                {"apply",     apply},
                {"mat3",      mat3},
                {"mat4",      mat4},
                {"mix",       mix},
                {"normalize", normalize},
                {"rotate",    rotate},
                {NULL, NULL}
        };

        static const luaL_reg quatlib_m[] = {
                {"__gc",       m_gc},
                {"__eq",       m_eq},
                {"__tostring", m_tostring},
                {"__unm",      m_unm},
                {"__len",      m_len},
                {"__add",      m_add},
                {"__mul",      m_mul},
                {"table",      table},
                {"get",        get},
                {"set",        set},
                {"setAll",     setAll},
                {"conjugate",  conjugate},
                {"cross",      cross},
                {"dot",        dot},
                {"inverse",    inverse},
                {"length",     length},
                {"apply",      apply},
                {"mat3",       mat3},
                {"mat4",       mat4},
                {"mix",        mix},
                {"normalize",  normalize},
                {"rotate",     rotate},
                {NULL, NULL}
        };

        int luaopen_quat(lua_State* L)
        {
            luaL_newmetatable(L, W_MT_QUAT);

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);

            luaL_register(L, NULL, quatlib_m);

            luaL_register(L, "Quat", quatlib_f);
            return 1;
        }

        W_REGISTER_MODULE(luaopen_quat);
    }

    void pushValue(lua_State* L, const glm::quat& value)
    {
        auto* container = (binding::QuatContainer*) lua_newuserdata(L, sizeof(binding::QuatContainer));
        container->quat = new glm::quat(value);
        luaL_getmetatable(L, W_MT_QUAT);
        lua_setmetatable(L, -2);
    }

    glm::quat* luaW_checkquat(lua_State* L, int narg)
    {
        void* dataPtr = luaL_checkudata(L, narg, W_MT_QUAT);
        luaL_argcheck(L, dataPtr != nullptr, narg, "'Quat' expected");
        binding::QuatContainer* container = (binding::QuatContainer*) dataPtr;
        return container->quat;
    }
}