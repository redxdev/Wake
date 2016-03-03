#pragma once

#include <glm/fwd.hpp>

#include "glutil.h"

namespace wake
{
    class Uniform
    {
    public:
        Uniform();
        Uniform(GLuint shaderProgram, GLint location);

        GLuint getProgram() const;
        GLint getLocation() const;
        bool isError() const;

        void set1f(GLfloat x);
        void set2f(GLfloat x, GLfloat y);
        void set3f(GLfloat x, GLfloat y, GLfloat z);
        void set4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);

        void set1i(GLint x);
        void set2i(GLint x, GLint y);
        void set3i(GLint x, GLint y, GLint z);
        void set4i(GLint x, GLint y, GLint z, GLint w);

        void set1ui(GLuint x);
        void set2ui(GLuint x, GLuint y);
        void set3ui(GLuint x, GLuint y, GLuint z);
        void set4ui(GLuint x, GLuint y, GLuint z, GLuint w);

        void setVec2(const glm::vec2& xy);
        void setVec3(const glm::vec3& xyz);
        void setVec4(const glm::vec4& xyzw);

        void setMatrix2(const glm::mat2x2& m22);
        void setMatrix2x3(const glm::mat2x3& m23);
        void setMatrix2x4(const glm::mat2x4& m24);

        void setMatrix3x2(const glm::mat3x2& m32);
        void setMatrix3(const glm::mat3x3& m33);
        void setMatrix3x4(const glm::mat3x4& m34);

        void setMatrix4x2(const glm::mat4x2& m42);
        void setMatrix4x3(const glm::mat4x3& m43);
        void setMatrix4(const glm::mat4x4& m44);

    private:
        GLuint shaderProgram;
        GLint uniformLocation;
    };

    class Shader
    {
    public:
        static Shader* compile(const char* vertexSource, const char* fragmentSource);
        static void reset();

        Shader(const Shader& other);

        ~Shader();

        Shader& operator=(const Shader& other);

        void use();

        GLuint getProgram() const;
        GLuint getVertexShader() const;
        GLuint getFragmentShader() const;

        Uniform getUniform(const char* name);

    private:
        GLuint shaderProgram;
        GLuint vertexShader;
        GLuint fragmentShader;

        Shader(GLuint shaderProgram, GLuint vertexShader, GLuint fragmentShader);
    };
}