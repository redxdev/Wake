#pragma once

#include "model.h"
#include "luautil.h"
#include "pushvalue.h"

#define W_MT_MODEL ("wake.Model")

namespace wake
{
    namespace binding
    {
        int luaopen_model(lua_State* L);
    }

    void pushValue(lua_State* L, ModelPtr value);

    ModelPtr luaW_checkmodel(lua_State* L, int narg);
}