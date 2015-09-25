#include "scriptmanager.h"

#include <iostream>

#include "moduleregistry.h"

namespace wake
{
    ScriptManager& ScriptManager::get()
    {
        static ScriptManager instance;
        return instance;
    }

    ScriptManager::ScriptManager()
    {
    }

    ScriptManager::~ScriptManager()
    {
    }

    bool ScriptManager::startup()
    {
        state = luaL_newstate();
        luaL_openlibs(state);

        setPath(W_SCRIPT_PATH);

        W_MODULE_REGISTRY.registerAll(state);

        return true;
    }

    bool ScriptManager::shutdown()
    {
        lua_close(state);
        state = nullptr;
        return true;
    }

    bool ScriptManager::initializeScripts()
    {
        return doFile("config/cfg.lua");

    }

    void ScriptManager::setPath(const char* path)
    {
        lua_getglobal(state, "package");
        lua_pushstring(state, path);
        lua_setfield(state, -2, "path");
        lua_pop(state, 1);
    }

    bool ScriptManager::doFile(const char* path)
    {
        if (luaL_loadfile(state, path) != 0)
        {
            std::cout << "Unable to run script: " << lua_tostring(state, -1) << std::endl;
            return false;
        }

        if (lua_pcall(state, 0, 0, 0) != 0)
        {
            std::cout << "Unable to run script: " << lua_tostring(state, -1) << std::endl;
            return false;
        }

        return true;
    }

    bool ScriptManager::isActive() const
    {
        return state != nullptr;
    }

    lua_State* ScriptManager::getState()
    {
        return state;
    }
}