#include "wake.h"

namespace wake
{
    // getVersion() is defined in build.cpp.in

    static EngineMode engineMode = EngineMode::Invalid;
    static std::vector<std::string> arguments;

    void setEngineMode(EngineMode mode)
    {
        engineMode = mode;
    }

    EngineMode getEngineMode()
    {
        return engineMode;
    }

    void setEngineArguments(const std::vector<std::string>& args)
    {
        arguments = args;
    }

    const std::vector<std::string>& getEngineArguments()
    {
        return arguments;
    }
}