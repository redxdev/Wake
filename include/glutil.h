#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define W_GL_CHECK() (checkGLErrors(__FILE__, __LINE__))

namespace wake
{
    bool checkGLErrors(const char* file, int line);
}