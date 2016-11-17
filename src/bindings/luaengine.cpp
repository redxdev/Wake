#include "bindings/luaengine.h"
#include "bindings/luaevent.h"
#include "moduleregistry.h"

namespace wake
{
    namespace binding
    {
        static int isRunning(lua_State* L)
        {
            pushValue(L, W_ENGINE.isRunning());
            return 1;
        }

        static int getTime(lua_State* L)
        {
            pushValue(L, W_ENGINE.getTime());
            return 1;
        }

        static int stop(lua_State* L)
        {
            W_ENGINE.stop();
            return 0;
        }

        static int checkGLErrors(lua_State* L)
        {
            lua_Debug dbg;
            lua_getstack(L, 1, &dbg);
            lua_getinfo(L, "nSl", &dbg);
            lua_pushboolean(L, wake::checkGLErrors(dbg.short_src, dbg.currentline));
            return 1;
        }

        static int setClearColor(lua_State* L)
        {
            GLclampf r = (GLclampf) luaL_checknumber(L, 1);
            GLclampf g = (GLclampf) luaL_checknumber(L, 2);
            GLclampf b = (GLclampf) luaL_checknumber(L, 2);
            GLclampf a = (GLclampf) luaL_checknumber(L, 2);
            W_ENGINE.setClearColor(r, g, b, a);
            return 0;
        }

        static int setWindowSize(lua_State* L)
        {
            int width = luaL_checkinteger(L, 1);
            int height = luaL_checkinteger(L, 2);
            W_ENGINE.setWindowSize(width, height);
            return 0;
        }

        static int setWindowFullscreen(lua_State* L)
        {
            W_ENGINE.setWindowFullscreen(lua_toboolean(L, 1) == 1);
            return 0;
        }

        static int setWindowTitle(lua_State* L)
        {
            W_ENGINE.setWindowTitle(luaL_checkstring(L, 1));
            return 0;
        }

        static int getWindowSize(lua_State* L)
        {
            lua_pushinteger(L, W_ENGINE.getWindowWidth());
            lua_pushinteger(L, W_ENGINE.getWindowHeight());
            return 2;
        }

        static const struct luaL_reg wakelib_f[] = {
                {"isRunning",           isRunning},
                {"getTime",             getTime},
                {"checkGLErrors",       checkGLErrors},
                {"stop",                stop},
                {"setClearColor",       setClearColor},
                {"setWindowSize",       setWindowSize},
                {"setWindowFullscreen", setWindowFullscreen},
                {"setWindowTitle",      setWindowTitle},
                {"getWindowSize",       getWindowSize},
                {NULL, NULL}
        };

        int luaopen_engine(lua_State* L)
        {
            luaL_register(L, "engine", wakelib_f);

            lua_pushstring(L, "earlyTick");
            pushValue(L, W_ENGINE.EarlyTickEvent);
            lua_settable(L, -3);

            lua_pushstring(L, "lateTick");
            pushValue(L, W_ENGINE.LateTickEvent);
            lua_settable(L, -3);

            lua_pushstring(L, "tick");
            pushValue(L, W_ENGINE.TickEvent);
            lua_settable(L, -3);

            lua_pushstring(L, "quit");
            pushValue(L, W_ENGINE.QuitEvent);
            lua_settable(L, -3);

            return 1;
        }

        W_REGISTER_MODULE(luaopen_engine);
    }
}