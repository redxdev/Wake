#pragma once

#include "event.h"
#include "util.h"

namespace wake
{
    enum class EngineMode : uint8
    {
        // No mode has been set yet
        INVALID,

        // The engine is running normally
        NORMAL,

        // The engine is running tests
        TESTING
    };

    const char* getVersion();

    void setEngineMode(EngineMode mode);

    EngineMode getEngineMode();
}