#pragma once

#include "event.h"
#include "util.h"

namespace wake
{
    enum class EngineMode
    {
        Invalid, // No mode has been set yet
        Normal, // The engine is running normally
        Testing // The engine is running tests
    };

    const char* getVersion();

    void setEngineMode(EngineMode mode);

    EngineMode getEngineMode();
}