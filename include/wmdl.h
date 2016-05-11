#pragma once

#include "model.h"
#include "util.h"

#define W_MDL_CODE "WMDL3"
#define W_MDL_MIN_VERSION ((wake::uint32) 3)
#define W_MDL_MAX_VERSION ((wake::uint32) 4)
#define W_MDL_VERSION ((wake::uint32) 4)

#define W_MDL_FLAG_NONE ((wake::uint64) 0)
#define W_MDL_FLAG_COMPRESS ((wake::uint64) 0x01)

namespace wake
{
    bool saveWMDL(const char* path, ModelPtr model, bool compress = true);

    ModelPtr loadWMDL(const char* path);
}