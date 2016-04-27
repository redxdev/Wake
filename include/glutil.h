#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef W_NO_GL_CHECKS
#define W_GL_CHECK()
#else
#define W_GL_CHECK() (checkGLErrors(__FILE__, __LINE__))
#endif

namespace wake
{
    bool checkGLErrors(const char* file, int line);
}