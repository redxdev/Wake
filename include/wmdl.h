#pragma once

#include "mesh.h"

#define W_MDL_CODE "WMDL3"
#define W_MDL_VERSION ((uint32_t) 3)

#define W_MDL_FLAG_NONE ((uint64_t) 0)
#define W_MDL_FLAG_COMPRESS ((uint64_t) 0x01)

namespace wake
{
    bool saveWMDL(const char* path, const std::vector<Mesh*>& meshes, bool compress = true);

    bool loadWMDL(const char* path, std::vector<Mesh*>& meshes);
}