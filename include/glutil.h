#pragma once

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#ifdef W_NO_GL_CHECKS
#define W_GL_CHECK()
#else
#define W_GL_CHECK() (checkGLErrors(__FILE__, __LINE__))
#endif

namespace wake
{
    const char* getGLErrorString(GLenum err);

    bool checkGLErrors(const char* file, int line);
}