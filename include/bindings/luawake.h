#pragma once

#include "wake.h"
#include "luautil.h"
#include "pushvalue.h"

namespace wake
{
    namespace binding
    {
        int luaopen_wake(lua_State* L);
    }
}