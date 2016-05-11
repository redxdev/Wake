#pragma once

#include "material.h"
#include "luautil.h"
#include "pushvalue.h"

#define W_MT_MATERIAL ("wake.Material")

namespace wake
{
    namespace binding
    {
        int luaopen_material(lua_State* L);
    }

    void pushValue(lua_State* L, MaterialPtr value);

    MaterialPtr luaW_checkmaterial(lua_State* L, int narg);
}