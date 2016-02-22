#pragma once

#include "shader.h"
#include "luautil.h"
#include "pushvalue.h"

#define W_MT_SHADER ("wake.Shader")
#define W_MT_UNIFORM ("wake.Uniform")

namespace wake
{
    namespace binding
    {
        int luaopen_shader(lua_State* L);

        int luaopen_uniform(lua_State* L);
    }

    void pushValue(lua_State* L, Shader* value);

    void pushValue(lua_State* L, const Uniform& value);

    Shader* luaW_checkshader(lua_State* L, int narg);

    Uniform luaW_checkuniform(lua_State* L, int narg);
}