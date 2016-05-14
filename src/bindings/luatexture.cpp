#include "bindings/luatexture.h"
#include "moduleregistry.h"

#include <sstream>
#include <cstring>

namespace wake
{
    namespace binding
    {
        struct TextureContainer
        {
            TexturePtr texture;
        };

        static int getSize(lua_State* L)
        {
            TexturePtr texture = luaW_checktexture(L, 1);
            lua_pushinteger(L, (lua_Integer) texture->getWidth());
            lua_pushinteger(L, (lua_Integer) texture->getHeight());
            return 2;
        }

        static int getComponentsPerPixel(lua_State* L)
        {
            TexturePtr texture = luaW_checktexture(L, 1);
            lua_pushinteger(L, (lua_Integer) texture->getComponentsPerPixel());
            return 1;
        }

        static int getPath(lua_State* L)
        {
            TexturePtr texture = luaW_checktexture(L, 1);
            lua_pushstring(L, texture->getPath().data());
            return 1;
        }

        static int generateMipMaps(lua_State* L)
        {
            TexturePtr texture = luaW_checktexture(L, 1);
            texture->generateMipMaps();
            return 0;
        }

        static int activate(lua_State* L)
        {
            TexturePtr texture = luaW_checktexture(L, 1);
            GLuint unit = (GLuint) luaL_checkinteger(L, 2);

            texture->activate(unit);
            return 0;
        }

        static int m_tostring(lua_State* L)
        {
            TexturePtr texture = luaW_checktexture(L, 1);
            std::stringstream ss;
            ss << "Texture(" << texture->getWidth() << "," << texture->getHeight() << ","
            << texture->getComponentsPerPixel() << ")";
            auto str = ss.str();
            lua_pushstring(L, str.data());
            return 1;
        }

        static int m_gc(lua_State* L)
        {
            void* dataPtr = luaL_checkudata(L, 1, W_MT_TEXTURE);
            luaL_argcheck(L, dataPtr != nullptr, 1, "'Texture' expected");
            TextureContainer* container = (TextureContainer*) dataPtr;
            container->texture.reset();
            return 0;
        }

        static const struct luaL_reg texturelib_f[] = {
                {"getSize",               getSize},
                {"getComponentsPerPixel", getComponentsPerPixel},
                {"getPath",               getPath},
                {"generateMipMaps",       generateMipMaps},
                {"use",                   activate},
                {NULL, NULL}
        };

        static const struct luaL_reg texturelib_m[] = {
                {"getSize",               getSize},
                {"getComponentsPerPixel", getComponentsPerPixel},
                {"getPath",               getPath},
                {"generateMipMaps",       generateMipMaps},
                {"use",                   activate},
                {"__gc",                  m_gc},
                {"__tostring",            m_tostring},
                {NULL, NULL}
        };

        int luaopen_texture(lua_State* L)
        {
            luaL_newmetatable(L, W_MT_TEXTURE);

            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2);
            lua_settable(L, -3);

            luaL_register(L, NULL, texturelib_m);

            luaL_register(L, "Texture", texturelib_f);
            return 1;
        }

        W_REGISTER_MODULE(luaopen_texture);
    }

    void pushValue(lua_State* L, TexturePtr value)
    {
        if (value.get() == nullptr)
        {
            lua_pushnil(L);
            return;
        }

        auto* container = (binding::TextureContainer*) lua_newuserdata(L, sizeof(binding::TextureContainer));
        memset(container, 0, sizeof(binding::TextureContainer));
        container->texture = value;
        luaL_getmetatable(L, W_MT_TEXTURE);
        lua_setmetatable(L, -2);
    }

    TexturePtr luaW_checktexture(lua_State* L, int narg)
    {
        void* dataPtr = luaL_checkudata(L, narg, W_MT_TEXTURE);
        luaL_argcheck(L, dataPtr != nullptr, narg, "'Texture' expected");
        binding::TextureContainer* container = (binding::TextureContainer*) dataPtr;
        return container->texture;
    }
}