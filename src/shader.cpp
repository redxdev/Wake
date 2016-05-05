#include "shader.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace wake
{
    Uniform::Uniform()
        : shaderProgram(0), uniformLocation(-1)
    {

    }

    Uniform::Uniform(GLuint shaderProgram, GLint location)
        : shaderProgram(shaderProgram), uniformLocation(location)
    {

    }

    GLuint Uniform::getProgram() const
    {
        return shaderProgram;
    }

    GLint Uniform::getLocation() const
    {
        return uniformLocation;
    }

    bool Uniform::isError() const
    {
        return uniformLocation == -1;
    }

    void Uniform::set1f(GLfloat x)
    {
        glUniform1f(uniformLocation, x);
    }

    void Uniform::set2f(GLfloat x, GLfloat y)
    {
        glUniform2f(uniformLocation, x, y);
    }

    void Uniform::set3f(GLfloat x, GLfloat y, GLfloat z)
    {
        glUniform3f(uniformLocation, x, y, z);
    }

    void Uniform::set4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        glUniform4f(uniformLocation, x, y, z, w);
    }

    void Uniform::set1i(GLint x)
    {
        glUniform1i(uniformLocation, x);
    }

    void Uniform::set2i(GLint x, GLint y)
    {
        glUniform2i(uniformLocation, x, y);
    }

    void Uniform::set3i(GLint x, GLint y, GLint z)
    {
        glUniform3i(uniformLocation, x, y, z);
    }

    void Uniform::set4i(GLint x, GLint y, GLint z, GLint w)
    {
        glUniform4i(uniformLocation, x, y, z, w);
    }

    void Uniform::set1ui(GLuint x)
    {
        glUniform1ui(uniformLocation, x);
    }

    void Uniform::set2ui(GLuint x, GLuint y)
    {
        glUniform2ui(uniformLocation, x, y);
    }

    void Uniform::set3ui(GLuint x, GLuint y, GLuint z)
    {
        glUniform3ui(uniformLocation, x, y, z);
    }

    void Uniform::set4ui(GLuint x, GLuint y, GLuint z, GLuint w)
    {
        glUniform4ui(uniformLocation, x, y, z, w);
    }

    void Uniform::setVec2(const glm::vec2& xy)
    {
        glUniform2fv(uniformLocation, 1, glm::value_ptr(xy));
    }

    void Uniform::setVec3(const glm::vec3& xyz)
    {
        glUniform3fv(uniformLocation, 1, glm::value_ptr(xyz));
    }

    void Uniform::setVec4(const glm::vec4& xyzw)
    {
        glUniform2fv(uniformLocation, 1, glm::value_ptr(xyzw));
    }

    void Uniform::setMatrix2(const glm::mat2x2& m22)
    {
        glUniformMatrix2fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m22));
    }

    void Uniform::setMatrix2x3(const glm::mat2x3& m23)
    {
        glUniformMatrix2x3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m23));
    }

    void Uniform::setMatrix2x4(const glm::mat2x4& m24)
    {
        glUniformMatrix2x4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m24));
    }

    void Uniform::setMatrix3x2(const glm::mat3x2& m32)
    {
        glUniformMatrix3x2fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m32));
    }

    void Uniform::setMatrix3(const glm::mat3x3& m33)
    {
        glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m33));
    }

    void Uniform::setMatrix3x4(const glm::mat3x4& m34)
    {
        glUniformMatrix3x4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m34));
    }

    void Uniform::setMatrix4x2(const glm::mat4x2& m42)
    {
        glUniformMatrix4x2fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m42));
    }

    void Uniform::setMatrix4x3(const glm::mat4x3& m43)
    {
        glUniformMatrix4x3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m43));
    }

    void Uniform::setMatrix4(const glm::mat4x4& m44)
    {
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m44));
    }

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

    void Shader::reset()
    {
        glUseProgram(0);
    }

    Shader::Shader(GLuint shaderProgram, GLuint vertexShader, GLuint fragmentShader)
        : shaderProgram(shaderProgram), vertexShader(vertexShader), fragmentShader(fragmentShader)
    {
    }

    Shader::Shader(const Shader& other)
        : vertexShader(other.vertexShader), fragmentShader(other.fragmentShader)
    {
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
    }

    Shader& Shader::operator=(const Shader& other)
    {
        vertexShader = other.vertexShader;
        fragmentShader = other.fragmentShader;

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        
        return *this;
    }

    Shader::~Shader()
    {
        glDeleteProgram(shaderProgram);
    }

    void Shader::use()
    {
        glUseProgram(shaderProgram);
    }

    GLuint Shader::getProgram() const
    {
        return shaderProgram;
    }

    GLuint Shader::getVertexShader() const
    {
        return vertexShader;
    }

    GLuint Shader::getFragmentShader() const
    {
        return fragmentShader;
    }

    Uniform Shader::getUniform(const char* name)
    {
        GLint location = glGetUniformLocation(shaderProgram, name);
        return Uniform(shaderProgram, location);
    }
}
