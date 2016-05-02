#pragma once

#include "mesh.h"

#define W_MDL_CODE "WMDL2"

#define W_MDL_FLAG_NONE 0
#define W_MDL_FLAG_COMPRESS 2

namespace wake
{
    bool saveWMDL(const char* path, const std::vector<Mesh*>& meshes, bool compress = true);

    bool loadWMDL(const char* path, std::vector<Mesh*>& meshes);
}