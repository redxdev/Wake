#pragma once

#include "texture.h"
#include "luautil.h"
#include "pushvalue.h"

#define W_MT_TEXTURE ("wake.Texture")

namespace wake
{
    namespace binding
    {
        int luaopen_texture(lua_State* L);
    }

    void pushValue(lua_State* L, Texture* value);

    Texture* luaW_checktexture(lua_State* L, int narg);
}