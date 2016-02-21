#pragma once

#include "glutil.h"

namespace wake
{
    class Shader
    {
    public:
        static Shader* compile(const char* vertexSource, const char* fragmentSource);

        ~Shader();

    private:
        GLuint shaderProgram;
        GLuint vertexShader;
        GLuint fragmentShader;

        Shader(GLuint shaderProgram, GLuint vertexShader, GLuint fragmentShader);
    };
}