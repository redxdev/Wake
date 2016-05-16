#include "bindings/luamaterial.h"
#include "bindings/luatexture.h"
#include "bindings/luashader.h"
#include "bindings/luamatrix.h"
#include "moduleregistry.h"

#include <cstring>

namespace wake
{
    namespace binding
    {
        struct MaterialContainer
        {
            MaterialPtr material;
        };

        static int getGlobal(lua_State* L)
        {
            pushValue(L, Material::getGlobalMaterial());
            return 1;
        }

        static int material_new(lua_State* L)
        {
            pushValue(L, MaterialPtr(new Material()));
            return 1;
        }

        static int getTypeName(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            lua_pushstring(L, material->getTypeName().data());
            return 1;
        }

        static int setTypeName(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            const char* name = luaL_checkstring(L, 2);
            material->setTypeName(name);
            return 1;
        }

        static int getShader(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            ShaderPtr shader = material->getShader();
            pushValue(L, shader);
            return 1;
        }

        static int setShader(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            ShaderPtr shader = luaW_checkshader(L, 2);
            material->setShader(shader);
            return 0;
        }

        static int setTexture(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            const char* name = luaL_checkstring(L, 2);
            TexturePtr texture = luaW_checktexture(L, 3);
            material->setTexture(name, texture);
            return 0;
        }

        static int removeTexture(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            const char* name = luaL_checkstring(L, 2);
            material->removeTexture(name);
            return 0;
        }

        static int getTexture(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            const char* name = luaL_checkstring(L, 2);
            pushValue(L, material->getTexture(name));
            return 1;
        }

        static int getTextures(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            auto& textures = material->getTextures();

            lua_newtable(L);
            for (auto entry : textures)
            {
                lua_pushstring(L, entry.first.data());
                pushValue(L, entry.second.texture);
                lua_settable(L, -3);
            }

            return 1;
        }

        static int getTextureCount(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            lua_pushinteger(L, (lua_Integer) material->getTextureCount());
            return 1;
        }

        static int setInt(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            const char* name = luaL_checkstring(L, 2);
            GLint i = (GLint) luaL_checkinteger(L, 3);
            material->setParameter(name, (GLint) i);
            return 0;
        }

        static int setUInt(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            const char* name = luaL_checkstring(L, 2);
            GLuint u = (GLuint) luaL_checkinteger(L, 3);
            material->setParameter(name, (GLuint) u);
            return 0;
        }

        static int setFloat(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            const char* name = luaL_checkstring(L, 2);
            GLfloat f = (GLfloat) luaL_checknumber(L, 3);
            material->setParameter(name, (GLfloat) f);
            return 0;
        }

        static int setVec2(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            const char* name = luaL_checkstring(L, 2);
            auto& vec = *luaW_checkvector2(L, 3);
            material->setParameter(name, vec);
            return 0;
        }

        static int setVec3(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            const char* name = luaL_checkstring(L, 2);
            auto& vec = *luaW_checkvector3(L, 3);
            material->setParameter(name, vec);
            return 0;
        }

        static int setVec4(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            const char* name = luaL_checkstring(L, 2);
            auto& vec = *luaW_checkvector4(L, 3);
            material->setParameter(name, vec);
            return 0;
        }

        static int setMatrix4(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            const char* name = luaL_checkstring(L, 2);
            auto& mat = *luaW_checkmatrix4x4(L, 3);
            material->setParameter(name, mat);
            return 0;
        }

        static int removeParameter(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            const char* name = luaL_checkstring(L, 2);
            material->removeParameter(name);
            return 0;
        }

        static int getParameter(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            const char* name = luaL_checkstring(L, 2);
            auto& parameter = material->getParameter(name);
            switch (parameter.type)
            {
                default:
                case MaterialParameter::Null:
                    lua_pushnil(L);
                    break;

                case MaterialParameter::Int:
                    lua_pushinteger(L, (lua_Integer) parameter.i);
                    break;

                case MaterialParameter::UInt:
                    lua_pushinteger(L, (lua_Integer) parameter.u);
                    break;

                case MaterialParameter::Float:
                    lua_pushnumber(L, (lua_Number) parameter.f);
                    break;

                case MaterialParameter::Vec2:
                    pushValue(L, parameter.v2);
                    break;

                case MaterialParameter::Vec3:
                    pushValue(L, parameter.v3);
                    break;

                case MaterialParameter::Vec4:
                    pushValue(L, parameter.v4);
                    break;

                case MaterialParameter::Mat4:
                    pushValue(L, parameter.m4);
                    break;
            }

            return 1;
        }

        static int getParameters(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            auto& parameters = material->getParameters();

            lua_newtable(L);
            for (auto entry : parameters)
            {
                lua_pushstring(L, entry.first.data());

                auto& parameter = entry.second;
                switch (parameter.type)
                {
                    default:
                    case MaterialParameter::Null:
                        lua_pushnil(L);
                        break;

                    case MaterialParameter::Int:
                        lua_pushinteger(L, (lua_Integer) parameter.i);
                        break;

                    case MaterialParameter::UInt:
                        lua_pushinteger(L, (lua_Integer) parameter.u);
                        break;

                    case MaterialParameter::Float:
                        lua_pushnumber(L, (lua_Number) parameter.f);
                        break;

                    case MaterialParameter::Vec2:
                        pushValue(L, parameter.v2);
                        break;

                    case MaterialParameter::Vec3:
                        pushValue(L, parameter.v3);
                        break;

                    case MaterialParameter::Vec4:
                        pushValue(L, parameter.v4);
                        break;

                    case MaterialParameter::Mat4:
                        pushValue(L, parameter.m4);
                        break;
                }

                lua_settable(L, -3);
            }

            return 1;
        }

        static int getParameterCount(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            lua_pushinteger(L, (lua_Integer) material->getParameterCount());
            return 1;
        }

        static int copyFrom(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            MaterialPtr other = luaW_checkmaterial(L, 2);
            material->copyFrom(other);
            return 0;
        }

        static int use(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            material->use();
            return 0;
        }

        static int clone(lua_State* L)
        {
            MaterialPtr material = luaW_checkmaterial(L, 1);
            pushValue(L, MaterialPtr(new Material(*material.get())));
            return 1;
        }

        static int m_tostring(lua_State* L)
        {
            lua_pushstring(L, "Material");
            return 1;
        }

        static int m_gc(lua_State* L)
        {
            void* dataPtr = luaL_checkudata(L, 1, W_MT_MATERIAL);
            luaL_argcheck(L, dataPtr != nullptr, 1, "'Material' expected");
            MaterialContainer* container = (MaterialContainer*) dataPtr;
            container->material.reset();
            return 0;
        }

        static const struct luaL_reg materiallib_f[] = {
                {"getGlobal",         getGlobal},
                {"new",               material_new},
                {"getTypeName",       getTypeName},
                {"setTypeName",       setTypeName},
                {"getShader",         getShader},
                {"setShader",         setShader},
                {"setTexture",        setTexture},
                {"removeTexture",     removeTexture},
                {"getTexture",        getTexture},
                {"getTextures",       getTextures},
                {"getTextureCount",   getTextureCount},
                {"setInt",            setInt},
                {"setUInt",           setUInt},
                {"setFloat",          setFloat},
                {"setVec2",           setVec2},
                {"setVec3",           setVec3},
                {"setVec4",           setVec4},
                {"setMatrix4",        setMatrix4},
                {"removeParameter",   removeParameter},
                {"getParameter",      getParameter},
                {"getParameters",     getParameters},
                {"getParameterCount", getParameterCount},
                {"copyFrom",          copyFrom},
                {"use",               use},
                {"clone",             clone},
                {NULL, NULL}
        };

        static const struct luaL_reg materiallib_m[] = {
                {"getTypeName",       getTypeName},
                {"setTypeName",       setTypeName},
                {"getShader",         getShader},
                {"setShader",         setShader},
                {"setTexture",        setTexture},
                {"removeTexture",     removeTexture},
                {"getTexture",        getTexture},
                {"getTextures",       getTextures},
                {"getTextureCount",   getTextureCount},
                {"setInt",            setInt},
                {"setUInt",           setUInt},
                {"setFloat",          setFloat},
                {"setVec2",           setVec2},
                {"setVec3",           setVec3},
                {"setVec4",           setVec4},
                {"setMatrix4",        setMatrix4},
                {"removeParameter",   removeParameter},
                {"getParameter",      getParameter},
                {"getParameters",     getParameters},
                {"getParameterCount", getParameterCount},
                {"copyFrom",          copyFrom},
                {"use",               use},
                {"clone",             clone},
                {"__tostring",        m_tostring},
                {"__gc",              m_gc},
                {NULL, NULL}
        };

        int luaopen_material(lua_State* L)
        {
            luaL_newmetatable(L, W_MT_MATERIAL);

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);

            luaL_register(L, NULL, materiallib_m);

            luaL_register(L, "Material", materiallib_f);
            return 1;
        }

        W_REGISTER_MODULE(luaopen_material);
    }

    void pushValue(lua_State* L, MaterialPtr value)
    {
        if (value.get() == nullptr)
        {
            lua_pushnil(L);
            return;
        }

        auto* container = (binding::MaterialContainer*) lua_newuserdata(L, sizeof(binding::MaterialContainer));
        memset(container, 0, sizeof(binding::MaterialContainer));
        container->material = value;
        luaL_getmetatable(L, W_MT_MATERIAL);
        lua_setmetatable(L, -2);
    }

    MaterialPtr luaW_checkmaterial(lua_State* L, int narg)
    {
        void* dataPtr = luaL_checkudata(L, narg, W_MT_MATERIAL);
        luaL_argcheck(L, dataPtr != nullptr, narg, "'Material' expected");
        binding::MaterialContainer* container = (binding::MaterialContainer*) dataPtr;
        return container->material;
    }
}