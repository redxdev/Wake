#pragma once

#include "mesh.h"
#include "luautil.h"
#include "pushvalue.h"

#define W_MT_MESH ("wake.Mesh")
#define W_MT_VERTEX ("wake.Vertex")

namespace wake
{
    namespace binding
    {
        int luaopen_mesh(lua_State* L);

        int luaopen_vertex(lua_State* L);
    }

    void pushValue(lua_State* L, Mesh* value);

    void pushValue(lua_State* L, const Vertex& value);

    Mesh* luaW_checkmesh(lua_State* L, int narg);

    Vertex* luaW_checkvertex(lua_State* L, int narg);
}