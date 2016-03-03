#include <iostream>
#include <tclap/CmdLine.h>

#include "wake.h"
#include "scriptmanager.h"
#include "engine.h"

int execute(bool testing)
{
    if (testing)
    {
        std::cout << "Running in testing mode." << std::endl;
        wake::setEngineMode(wake::EngineMode::TESTING);
    }
    else
    {
        wake::setEngineMode(wake::EngineMode::NORMAL);
    }

    if (!W_SCRIPT.startup())
    {
        std::cout << "Unable to start scripting engine." << std::endl;
        return 1;
    }

    if (!W_SCRIPT.initializeScripts())
    {
        std::cout << "Unable to load configuration." << std::endl;
        W_SCRIPT.shutdown();
        return 1;
    }

    if (testing)
    {
        ////
        // Test suite
        ////

        std::cout << "Loading tests..." << std::endl;
        if (!W_SCRIPT.doFile("tests/init.lua"))
        {
            std::cout << "Unable to load tests." << std::endl;
            return 1;
        }

        std::cout << "Running tests..." << std::endl << std::endl;
        lua_State* state = W_SCRIPT.getState();
        lua_getglobal(state, "hook_engine_tests");

        if (lua_pcall(state, 0, 1, 0) != 0)
        {
            std::cout << "Unable to run tests: " << lua_tostring(state, -1) << std::endl;
            return 1;
        }

        if (!lua_isboolean(state, -1))
        {
            std::cout << "hook_engine_tests must return a boolean" << std::endl;
            return 1;
        }

        bool result = lua_toboolean(state, -1) != 0;

        W_SCRIPT.shutdown();

        if (result)
        {
            std::cout << "All tests passed successfully." << std::endl;
            return 0;
        }
        else
        {
            std::cout << "All tests did not pass successfully." << std::endl;
            return 1;
        }
    }
    else
    {
        ////
        // Normal Execution
        ////

        if (!W_ENGINE.startup())
        {
            std::cout << "Unable to start engine." << std::endl;
            return 1;
        }

        if (!W_SCRIPT.doFile("game/init.lua"))
        {
            std::cout << "Unable to start game." << std::endl;
            W_SCRIPT.shutdown();
            return 1;
        }

        int result = 0;
        if (!W_ENGINE.run())
        {
            std::cout << "Unable to run game." << std::endl;
            result = 1;
        }

        W_ENGINE.shutdown();
        W_SCRIPT.shutdown();

        return result;
    }
}

int main(int argc, char** argv)
{
    int result = 0;
    bool pause = false;
    try
    {
        TCLAP::CmdLine cmd("Wake game engine", ' ', wake::getVersion());

        TCLAP::SwitchArg testingArg("t", "testing", "Run in testing mode. This will launch the test scripts instead of the game scripts.", cmd,
                                    false);
        TCLAP::SwitchArg pauseArg("p", "pause", "Pause execution on exit.", cmd, false);

        cmd.parse(argc, argv);

        pause = pauseArg.getValue();

        result = execute(testingArg.getValue());
    }
    catch (TCLAP::ArgException& e)
    {
        std::cout << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        result = 1;
    }

    if (pause)
    {
        std::cout << "Press any key to continue..." << std::endl;
        getchar();
    }

    return result;
}