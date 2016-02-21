#include "bindings/luashader.h"
#include "moduleregistry.h"

namespace wake
{
    namespace binding
    {
        struct ShaderContainer
        {
            Shader* shader;
        };

        static int shader_new(lua_State* L)
        {
            const char* vertexSource = luaL_checkstring(L, 1);
            const char* fragmentSource = luaL_checkstring(L, 2);
            Shader* shader = Shader::compile(vertexSource, fragmentSource);
            if (shader == nullptr)
            {
                lua_pushnil(L);
                return 1;
            }

            pushValue(L, shader);
            return 1;
        }

        static int m_gc(lua_State* L)
        {
            delete luaW_checkshader(L, 1);
            return 0;
        }

        static int m_tostring(lua_State* L)
        {
            lua_pushstring(L, "Shader");
            return 1;
        }

        static const struct luaL_reg shaderlib_f[] = {
                {"new",     shader_new},
                {NULL, NULL}
        };

        static const struct luaL_reg shaderlib_m[] = {
                {"__gc",    m_gc},
                {"__tostring",  m_tostring},
                {NULL, NULL}
        };

        int luaopen_shader(lua_State* L)
        {
            luaL_newmetatable(L, W_MT_SHADER);

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);

            luaL_register(L, NULL, shaderlib_m);

            luaL_register(L, "Shader", shaderlib_f);
            return 1;
        }

        W_REGISTER_MODULE(luaopen_shader);
    }

    void pushValue(lua_State* L, Shader* value)
    {
        auto* container = (binding::ShaderContainer*) lua_newuserdata(L, sizeof(binding::ShaderContainer));
        container->shader = value;
        luaL_getmetatable(L, W_MT_SHADER);
        lua_setmetatable(L, -2);
    }

    Shader* luaW_checkshader(lua_State* L, int narg)
    {
        void* dataPtr = luaL_checkudata(L, narg, W_MT_SHADER);
        luaL_argcheck(L, dataPtr != nullptr, narg, "'Shader' expected");
        binding::ShaderContainer* container = (binding::ShaderContainer*) dataPtr;
        return container->shader;
    }
}