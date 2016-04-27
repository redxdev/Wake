#include "wake.h"

namespace wake
{
    // getVersion() is defined in build.cpp.in

    static EngineMode engineMode = EngineMode::Invalid;

    void setEngineMode(EngineMode mode)
    {
        engineMode = mode;
    }

    EngineMode getEngineMode()
    {
        return engineMode;
    }
}