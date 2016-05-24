#pragma once

#include "luautil.h"

#define W_SCRIPT (wake::ScriptManager::get())
#define W_SCRIPT_PATH ("?;?.lua;game/?.lua;game/?;lib/?.lua;lib/?")

namespace wake
{
    class ScriptManager
    {
    public:
        static ScriptManager& get();

    public:
        bool startup();

        bool shutdown();

        bool initializeScripts();

        void setPath(const char* path);

        bool doFile(const char* path);

        bool isActive() const;

        lua_State* getState();

    private:
        ScriptManager();

        ScriptManager(const ScriptManager& other) = delete;

        ScriptManager& operator=(const ScriptManager& other) = delete;

        ~ScriptManager();

        lua_State* state = nullptr;
    };
}