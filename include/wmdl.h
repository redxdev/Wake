#pragma once

#include "mesh.h"
#include "util.h"

#define W_MDL_CODE "WMDL3"
#define W_MDL_VERSION ((wake::uint32) 3)

#define W_MDL_FLAG_NONE ((wake::uint64) 0)
#define W_MDL_FLAG_COMPRESS ((wake::uint64) 0x01)

namespace wake
{
    bool saveWMDL(const char* path, const std::vector<Mesh*>& meshes, bool compress = true);

    bool loadWMDL(const char* path, std::vector<Mesh*>& meshes);
}