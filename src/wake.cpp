#include "wake.h"

namespace wake
{
    // getVersion() is defined in build.cpp.in

    static EngineMode engineMode = EngineMode::INVALID;

    void setEngineMode(EngineMode mode)
    {
        engineMode = mode;
    }

    EngineMode getEngineMode()
    {
        return engineMode;
    }
}