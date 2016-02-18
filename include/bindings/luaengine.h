#pragma once

#include "engine.h"
#include "luautil.h"
#include "pushvalue.h"

namespace wake
{
    namespace binding
    {
        int luaopen_engine(lua_State* L);
    }
}