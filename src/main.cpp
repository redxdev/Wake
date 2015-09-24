#include <iostream>
#include <tclap/CmdLine.h>

#include "wake.h"
#include "scriptmanager.h"


using namespace std;

int main(int argc, char** argv)
{
    try
    {
        TCLAP::CmdLine cmd("Wake game engine", ' ', wake::getVersion());

        TCLAP::SwitchArg testingArg("t", "testing", "Run in testing mode. This will launch the test scripts instead of the game scripts.", cmd,
                                    false);

        cmd.parse(argc, argv);

        if (testingArg.getValue())
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

        if (testingArg.getValue())
        {
            ////
            // Wake test suite
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
            if (!W_SCRIPT.doFile("game/init.lua"))
            {
                std::cout << "Unable to start game." << std::endl;
                W_SCRIPT.shutdown();
                return 1;
            }

            W_SCRIPT.shutdown();
        }
    }
    catch (TCLAP::ArgException& e)
    {
        std::cout << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }

    return 0;
}