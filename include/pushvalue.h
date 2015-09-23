#pragma once

#include "luautil.h"
#include "util.h"

#include <string>

namespace wake
{
    template<typename T, typename... Arguments>
    void pushValue(lua_State* L, T first, Arguments... args)
    {
        pushValue(L, first);
        pushValue(L, args...);
    }

    // Do nothing
    void pushValue(lua_State* L);

    void pushValue(lua_State* L, float value);

    void pushValue(lua_State* L, double value);

    void pushValue(lua_State* L, uint8 value);

    void pushValue(lua_State* L, uint16 value);

    void pushValue(lua_State* L, uint32 value);

    void pushValue(lua_State* L, uint64 value);

    void pushValue(lua_State* L, int8 value);

    void pushValue(lua_State* L, int16 value);

    void pushValue(lua_State* L, int32 value);

    void pushValue(lua_State* L, int64 value);

    void pushValue(lua_State* L, const char* value);

    void pushValue(lua_State* L, const std::string& value);

    void pushValue(lua_State* L, bool value);
}