#pragma once

#include "shader.h"
#include "luautil.h"
#include "pushvalue.h"

#define W_MT_SHADER ("wake.Shader")

namespace wake
{
    namespace binding
    {
        int luaopen_shader(lua_State* L);
    }

    void pushValue(lua_State* L, Shader* value);

    Shader* luaW_checkshader(lua_State* L, int narg);
}