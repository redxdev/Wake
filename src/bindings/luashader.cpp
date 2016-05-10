#include "bindings/luashader.h"
#include "bindings/luamatrix.h"
#include "moduleregistry.h"

#include <cstring>

namespace wake
{
    namespace binding
    {
        struct ShaderContainer
        {
            ShaderPtr shader;
        };

        static int shader_new(lua_State* L)
        {
            const char* vertexSource = luaL_checkstring(L, 1);
            const char* fragmentSource = luaL_checkstring(L, 2);
            ShaderPtr shader = Shader::compile(vertexSource, fragmentSource);
            if (shader == nullptr)
            {
                lua_pushnil(L);
                return 1;
            }

            pushValue(L, shader);
            return 1;
        }

        static int shader_reset(lua_State* L)
        {
            Shader::reset();
            return 0;
        }

        static int shader_use(lua_State* L)
        {
            ShaderPtr shader = luaW_checkshader(L, 1);
            shader->use();
            return 0;
        }

        static int shader_get_uniform(lua_State* L)
        {
            ShaderPtr shader = luaW_checkshader(L, 1);
            const char* name = luaL_checkstring(L, 2);
            Uniform uniform = shader->getUniform(name);
            if (uniform.isError())
            {
                lua_pushnil(L);
                return 1;
            }

            pushValue(L, uniform);
            return 1;
        }

        static int shader_m_gc(lua_State* L)
        {
            void* dataPtr = luaL_checkudata(L, 1, W_MT_SHADER);
            ShaderContainer* container = (ShaderContainer*) dataPtr;
            container->shader.reset();
            return 0;
        }

        static int shader_m_tostring(lua_State* L)
        {
            lua_pushstring(L, "Shader");
            return 1;
        }

        static const struct luaL_reg shaderlib_f[] = {
                {"new",         shader_new},
                {"reset",       shader_reset},
                {"getUniform",  shader_get_uniform},
                {"use",         shader_use},
                {NULL, NULL}
        };

        static const struct luaL_reg shaderlib_m[] = {
                {"getUniform",  shader_get_uniform},
                {"use",         shader_use},
                {"__gc",        shader_m_gc},
                {"__tostring",  shader_m_tostring},
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

        struct UniformContainer
        {
            Uniform uniform;
        };

        static int uniform_is_error(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);
            lua_pushboolean(L, uniform.isError());
            return 1;
        }

        static int uniform_set1f(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);

            GLfloat x = (GLfloat) luaL_checknumber(L, 2);
            uniform.set1f(x);

            return 0;
        }

        static int uniform_set2f(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);

            GLfloat x = (GLfloat) luaL_checknumber(L, 2);
            GLfloat y = (GLfloat) luaL_checknumber(L, 3);
            uniform.set2f(x, y);

            return 0;
        }

        static int uniform_set3f(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);

            GLfloat x = (GLfloat) luaL_checknumber(L, 2);
            GLfloat y = (GLfloat) luaL_checknumber(L, 3);
            GLfloat z = (GLfloat) luaL_checknumber(L, 4);
            uniform.set3f(x, y, z);

            return 0;
        }

        static int uniform_set4f(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);

            GLfloat x = (GLfloat) luaL_checknumber(L, 2);
            GLfloat y = (GLfloat) luaL_checknumber(L, 3);
            GLfloat z = (GLfloat) luaL_checknumber(L, 4);
            GLfloat w = (GLfloat) luaL_checknumber(L, 5);
            uniform.set4f(x, y, z, w);

            return 0;
        }

        static int uniform_set1i(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);

            GLint x = (GLint) luaL_checkinteger(L, 2);

            uniform.set1i(x);

            return 0;
        }

        static int uniform_set2i(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);

            GLint x = (GLint) luaL_checkinteger(L, 2);
            GLint y = (GLint) luaL_checkinteger(L, 3);

            uniform.set2i(x, y);

            return 0;
        }

        static int uniform_set3i(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);

            GLint x = (GLint) luaL_checkinteger(L, 2);
            GLint y = (GLint) luaL_checkinteger(L, 3);
            GLint z = (GLint) luaL_checkinteger(L, 4);

            uniform.set3i(x, y, z);

            return 0;
        }

        static int uniform_set4i(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);

            GLint x = (GLint) luaL_checkinteger(L, 2);
            GLint y = (GLint) luaL_checkinteger(L, 3);
            GLint z = (GLint) luaL_checkinteger(L, 4);
            GLint w = (GLint) luaL_checkinteger(L, 5);

            uniform.set4i(x, y, z, w);

            return 0;
        }

        static int uniform_set1ui(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);

            GLuint x = (GLuint) luaL_checkinteger(L, 2);

            uniform.set1ui(x);

            return 0;
        }

        static int uniform_set2ui(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);

            GLuint x = (GLuint) luaL_checkinteger(L, 2);
            GLuint y = (GLuint) luaL_checkinteger(L, 3);

            uniform.set2ui(x, y);

            return 0;
        }

        static int uniform_set3ui(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);

            GLuint x = (GLuint) luaL_checkinteger(L, 2);
            GLuint y = (GLuint) luaL_checkinteger(L, 3);
            GLuint z = (GLuint) luaL_checkinteger(L, 4);

            uniform.set3ui(x, y, z);

            return 0;
        }

        static int uniform_set4ui(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);

            GLuint x = (GLuint) luaL_checkinteger(L, 2);
            GLuint y = (GLuint) luaL_checkinteger(L, 3);
            GLuint z = (GLuint) luaL_checkinteger(L, 4);
            GLuint w = (GLuint) luaL_checkinteger(L, 5);

            uniform.set4ui(x, y, z, w);

            return 0;
        }

        static int uniform_setVec2(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);
            glm::vec2& vec = *luaW_checkvector2(L, 2);

            uniform.setVec2(vec);

            return 0;
        }

        static int uniform_setVec3(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);
            glm::vec3& vec = *luaW_checkvector3(L, 2);

            uniform.setVec3(vec);

            return 0;
        }

        static int uniform_setVec4(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);
            glm::vec4& vec = *luaW_checkvector4(L, 2);

            uniform.setVec4(vec);

            return 0;
        }

        static int uniform_setMatrix2(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);
            glm::mat2& mat = *luaW_checkmatrix2x2(L, 2);

            uniform.setMatrix2(mat);

            return 0;
        }

        static int uniform_setMatrix2x3(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);
            glm::mat2x3& mat = *luaW_checkmatrix2x3(L, 2);

            uniform.setMatrix2x3(mat);

            return 0;
        }

        static int uniform_setMatrix2x4(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);
            glm::mat2x4& mat = *luaW_checkmatrix2x4(L, 2);

            uniform.setMatrix2x4(mat);

            return 0;
        }

        static int uniform_setMatrix3x2(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);
            glm::mat3x2& mat = *luaW_checkmatrix3x2(L, 2);

            uniform.setMatrix3x2(mat);

            return 0;
        }

        static int uniform_setMatrix3(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);
            glm::mat3& mat = *luaW_checkmatrix3x3(L, 2);

            uniform.setMatrix3(mat);

            return 0;
        }

        static int uniform_setMatrix3x4(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);
            glm::mat3x4& mat = *luaW_checkmatrix3x4(L, 2);

            uniform.setMatrix3x4(mat);

            return 0;
        }

        static int uniform_setMatrix4x2(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);
            glm::mat4x2& mat = *luaW_checkmatrix4x2(L, 2);

            uniform.setMatrix4x2(mat);

            return 0;
        }

        static int uniform_setMatrix4x3(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);
            glm::mat4x3& mat = *luaW_checkmatrix4x3(L, 2);

            uniform.setMatrix4x3(mat);

            return 0;
        }

        static int uniform_setMatrix4(lua_State* L)
        {
            Uniform uniform = luaW_checkuniform(L, 1);
            glm::mat4& mat = *luaW_checkmatrix4x4(L, 2);

            uniform.setMatrix4(mat);

            return 0;
        }

        static int uniform_m_tostring(lua_State* L)
        {
            lua_pushstring(L, "Uniform");
            return 1;
        }

        static const struct luaL_reg uniformlib_f[] = {
                {"isError",         uniform_is_error},
                {"set1f",           uniform_set1f},
                {"set2f",           uniform_set2f},
                {"set3f",           uniform_set3f},
                {"set4f",           uniform_set4f},
                {"set1i",           uniform_set1i},
                {"set2i",           uniform_set2i},
                {"set3i",           uniform_set3i},
                {"set4i",           uniform_set4i},
                {"set1ui",          uniform_set1ui},
                {"set2ui",          uniform_set2ui},
                {"set3ui",          uniform_set3ui},
                {"set4ui",          uniform_set4ui},
                {"setVec2",         uniform_setVec2},
                {"setVec3",         uniform_setVec3},
                {"setVec4",         uniform_setVec4},
                {"setMatrix2",          uniform_setMatrix2},
                {"setMatrix2x3",       uniform_setMatrix2x3},
                {"setMatrix2x4",       uniform_setMatrix2x4},
                {"setMatrix3x2",       uniform_setMatrix3x2},
                {"setMatrix3",          uniform_setMatrix3},
                {"setMatrix3x4",       uniform_setMatrix3x4},
                {"setMatrix4x2",       uniform_setMatrix4x2},
                {"setMatrix4x3",       uniform_setMatrix4x3},
                {"setMatrix4",          uniform_setMatrix4},
                {NULL, NULL}
        };

        static const struct luaL_reg uniformlib_m[] = {
                {"isError",             uniform_is_error},
                {"set1f",               uniform_set1f},
                {"set2f",               uniform_set2f},
                {"set3f",               uniform_set3f},
                {"set4f",               uniform_set4f},
                {"set1i",               uniform_set1i},
                {"set2i",               uniform_set2i},
                {"set3i",               uniform_set3i},
                {"set4i",               uniform_set4i},
                {"set1ui",              uniform_set1ui},
                {"set2ui",              uniform_set2ui},
                {"set3ui",              uniform_set3ui},
                {"set4ui",              uniform_set4ui},
                {"setVec2",             uniform_setVec2},
                {"setVec3",             uniform_setVec3},
                {"setVec4",             uniform_setVec4},
                {"setMatrix2",          uniform_setMatrix2},
                {"setMatrix2x3",        uniform_setMatrix2x3},
                {"setMatrix2x4",        uniform_setMatrix2x4},
                {"setMatrix3x2",        uniform_setMatrix3x2},
                {"setMatrix3",          uniform_setMatrix3},
                {"setMatrix3x4",        uniform_setMatrix3x4},
                {"setMatrix4x2",        uniform_setMatrix4x2},
                {"setMatrix4x3",        uniform_setMatrix4x3},
                {"setMatrix4",          uniform_setMatrix4},
                {"__tostring",          uniform_m_tostring},
                {NULL, NULL}
        };

        int luaopen_uniform(lua_State* L)
        {
            luaL_newmetatable(L, W_MT_UNIFORM);

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);

            luaL_register(L, NULL, uniformlib_m);

            luaL_register(L, "Uniform", uniformlib_f);
            return 1;
        }

        W_REGISTER_MODULE(luaopen_uniform);
    }

    void pushValue(lua_State* L, ShaderPtr value)
    {
        auto* container = (binding::ShaderContainer*) lua_newuserdata(L, sizeof(binding::ShaderContainer));
        memset(container, 0, sizeof(binding::ShaderContainer));
        container->shader = value;
        luaL_getmetatable(L, W_MT_SHADER);
        lua_setmetatable(L, -2);
    }

    void pushValue(lua_State* L, const Uniform& value)
    {
        auto* container = (binding::UniformContainer*) lua_newuserdata(L, sizeof(binding::UniformContainer));
        container->uniform = value;
        luaL_getmetatable(L, W_MT_UNIFORM);
        lua_setmetatable(L, -2);
    }

    ShaderPtr luaW_checkshader(lua_State* L, int narg)
    {
        void* dataPtr = luaL_checkudata(L, narg, W_MT_SHADER);
        luaL_argcheck(L, dataPtr != nullptr, narg, "'Shader' expected");
        binding::ShaderContainer* container = (binding::ShaderContainer*) dataPtr;
        return container->shader;
    }

    Uniform luaW_checkuniform(lua_State* L, int narg)
    {
        void* dataPtr = luaL_checkudata(L, narg, W_MT_UNIFORM);
        luaL_argcheck(L, dataPtr != nullptr, narg, "'Uniform' expected");
        binding::UniformContainer* container = (binding::UniformContainer*) dataPtr;
        return container->uniform;
    }
}