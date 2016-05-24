#include "glutil.h"

#include <iostream>

namespace wake
{
    const char* getGLErrorString(GLenum err)
    {
        switch (err)
        {
            default:
                return "unknown error";

            case GL_NO_ERROR:
                return "no error";

            case GL_INVALID_ENUM:
                return "invalid enumeration";

            case GL_INVALID_VALUE:
                return "invalid value";

            case GL_INVALID_OPERATION:
                return "invalid operation";

            case GL_STACK_OVERFLOW:
                return "stack overflow";

            case GL_STACK_UNDERFLOW:
                return "stack underflow";

            case GL_OUT_OF_MEMORY:
                return "out of memory";
        }
    }

    bool checkGLErrors(const char* file, int line)
    {
        GLenum err;
        bool hasError = false;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            hasError = true;
            std::cout << file << ":" << line << " - " << getGLErrorString(err) << " (" << err << ")" << std::endl;
        }

        return hasError;
    }
}