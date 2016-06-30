#pragma once

#include "model.h"
#include "util.h"

// WMDL is a binaryfile format for storing model data. While wake can read any format supported by assimp, it can be
// slow to read assimp's data structures into something wake can actually make use of. The WMDL format is meant to
// mirror the format of the wake::Model class (and by extension wake::Material), and this implementation reads directly
// into a wake::Model, thereby reducing the amount of data that needs to be copied around.
//
// Whenever possible, this implementation of WMDL is meant to be forwards-compatible - newer implementations should
// generally be able to read files from older versions of WMDL (but not from newer versions). Versions supported for a
// specific implementation can be found by looking at the W_MDL_MIN_VERSION and W_MDL_MAX_VERSION defines below.
//
// This implementation of WMDL cannot write older version of the file format. wake::saveWMDL can only write the latest
// version of the format. A side effect of this is that you can upgrade old files by reading them via wake::loadWMDL and
// then saving them with wake::saveWMDL - the file will be saved as the newest version.
//
// WMDL files may be optionally compressed. The compression algorithm of choice is Google's Snappy and is the only
// algorithm currently supported. It is highly recommended to use compression (the default), as it tends to heavily
// reduce the file size and in many cases is faster to read than with uncompressed files (disk IO tends to slow things
// down more than decompression, so compressed files tend to be faster to load).

#define W_MDL_CODE "WMDL3"
#define W_MDL_MIN_VERSION ((wake::uint32) 3)
#define W_MDL_MAX_VERSION ((wake::uint32) 6)
#define W_MDL_VERSION ((wake::uint32) 6)

#define W_MDL_FLAG_NONE ((wake::uint64) 0)
#define W_MDL_FLAG_COMPRESS ((wake::uint64) (1 << 0))

namespace wake
{
    bool saveWMDL(const char* path, ModelPtr model, bool compress = true);

    ModelPtr loadWMDL(const char* path);
}