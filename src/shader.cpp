#include "shader.h"

#include <iostream>

namespace wake
{
    Shader* Shader::compile(const char* vertexSource, const char* fragmentSource)
    {
        bool ok = true;

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);

        GLint status;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE)
        {
            ok = false;
            char buffer[512];
            glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
            std::cout << "Vertex shader compile error:" << std::endl;
            std::cout << "\t" << buffer << std::endl;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE)
        {
            ok = false;
            char buffer[512];
            glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
            std::cout << "Fragment shader compile error:" << std::endl;
            std::cout << "\t" << buffer << std::endl;
        }

        if (!ok)
        {
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return nullptr;
        }

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        return new Shader(shaderProgram, vertexShader, fragmentShader);
    }

    Shader::Shader(GLuint shaderProgram, GLuint vertexShader, GLuint fragmentShader)
        : shaderProgram(shaderProgram), vertexShader(vertexShader), fragmentShader(fragmentShader)
    {
    }

    Shader::~Shader()
    {
        glDeleteProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
}