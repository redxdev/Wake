#pragma once

#include "glutil.h"
#include "texture.h"
#include "shader.h"
#include "luautil.h"
#include "util.h"
#include "shader.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace wake
{
    struct MaterialParameter
    {
        enum
        {
            Int,
            UInt,
            Float,
            Vec2,
            Vec3,
            Vec4
        } type;

        union
        {
            GLint i;
            GLuint u;
            GLfloat f;
            glm::vec2 v2;
            glm::vec3 v3;
            glm::vec4 v4;
        };
    };

    class Material
    {
    public:
        ShaderPtr getShader() const;

        const std::unordered_map<std::string, TexturePtr>& getTextures() const;

        const std::unordered_map<std::string, MaterialParameter> getParameters() const;

        void activate();

    private:
        ShaderPtr shader;
        std::unordered_map<std::string, TexturePtr> textures;
        std::unordered_map<std::string, MaterialParameter> parameters;
    };
}