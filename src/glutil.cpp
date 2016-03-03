#include "glutil.h"

#include <iostream>

namespace wake
{
    bool checkGLErrors(const char* file, int line)
    {
        GLenum err;
        bool hasError = false;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            hasError = true;
            std::cout << file << ":" << line << " - " << gluErrorString(err) << " (" << err << ")" << std::endl;
        }

        return hasError;
    }
}