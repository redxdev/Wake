#pragma once

#include "event.h"
#include "util.h"

#include <vector>
#include <string>

namespace wake
{
    enum class EngineMode
    {
        Invalid, // No mode has been set yet
        Normal, // The engine is running normally
        Testing, // The engine is running tests
        Tool // The engine is running tools
    };

    const char* getVersion();

    void setEngineMode(EngineMode mode);

    EngineMode getEngineMode();

    void setEngineArguments(const std::vector<std::string>& args);

    const std::vector<std::string>& getEngineArguments();
}