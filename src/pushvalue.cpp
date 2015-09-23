#include "pushvalue.h"

namespace wake
{
    void pushValue(lua_State* L)
    {
        // end of recursion, do nothing
    }

    void pushValue(lua_State* L, float value)
    {
        lua_pushnumber(L, value);
    }

    void pushValue(lua_State* L, double value)
    {
        lua_pushnumber(L, value);
    }

    void pushValue(lua_State* L, uint8 value)
    {
        lua_pushinteger(L, value);
    }

    void pushValue(lua_State* L, uint16 value)
    {
        lua_pushinteger(L, value);
    }

    void pushValue(lua_State* L, uint32 value)
    {
        lua_pushinteger(L, value);
    }

    void pushValue(lua_State* L, uint64 value)
    {
        lua_pushinteger(L, value);
    }

    void pushValue(lua_State* L, int8 value)
    {
        lua_pushinteger(L, value);
    }

    void pushValue(lua_State* L, int16 value)
    {
        lua_pushinteger(L, value);
    }

    void pushValue(lua_State* L, int32 value)
    {
        lua_pushinteger(L, value);
    }

    void pushValue(lua_State* L, int64 value)
    {
        lua_pushinteger(L, value);
    }

    void pushValue(lua_State* L, const char* value)
    {
        lua_pushstring(L, value);
    }

    void pushValue(lua_State* L, const std::string& value)
    {
        lua_pushstring(L, value.c_str());
    }

    void pushValue(lua_State* L, bool value)
    {
        lua_pushboolean(L, value);
    }
}