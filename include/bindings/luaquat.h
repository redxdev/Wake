#pragma once

#include "luautil.h"
#include "pushvalue.h"

#include <glm/glm.hpp>

#define W_MT_QUAT ("wake.Quat")

namespace wake
{
    namespace binding
    {
        int luaopen_quat(lua_State* L);
    }

    void pushValue(lua_State* L, const glm::quat& value);

    glm::quat* luaW_checkquat(lua_State* L, int narg);
}