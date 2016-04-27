#pragma once

#include "luautil.h"
#include "pushvalue.h"

namespace wake
{
    namespace binding
    {
        int luaopen_math(lua_State* L);
    }
}