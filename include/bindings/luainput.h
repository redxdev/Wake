#pragma once

#include "input.h"
#include "luautil.h"
#include "pushvalue.h"

namespace wake
{
    namespace binding
    {
        int luaopen_input(lua_State* L);
    }

    void pushValue(lua_State* L, InputAction value);

    void pushValue(lua_State* L, KeyboardInput value);

    void pushValue(lua_State* L, MouseInput value);

    void pushValue(lua_State* L, CursorMode value);
}