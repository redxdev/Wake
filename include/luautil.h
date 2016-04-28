#pragma once

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace wake
{
    namespace binding
    {
        bool checkMetatable(lua_State* L, int idx, const char* name);
    }
}