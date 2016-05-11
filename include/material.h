#pragma once

#include "glutil.h"
#include "texture.h"
#include "shader.h"
#include "luautil.h"
#include "util.h"
#include "shader.h"
#include "engineptr.h"

#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace wake
{
    struct MaterialParameter
    {
        static MaterialParameter NullParameter;

        MaterialParameter()
        {
            type = Null;
            i = 0;
        }

        enum
        {
            Null,
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
        Material();

        Material(const Material& other);

        ~Material();

        Material& operator=(const Material& other);

        void setShader(ShaderPtr shader);

        ShaderPtr getShader() const;

        void setTexture(const std::string& name, TexturePtr texture);

        void removeTexture(const std::string& name);

        TexturePtr getTexture(const std::string& name);

        const std::map<std::string, TexturePtr>& getTextures() const;

        void setParameter(const std::string& name, GLint i);

        void setParameter(const std::string& name, GLuint u);

        void setParameter(const std::string& name, GLfloat f);

        void setParameter(const std::string& name, const glm::vec2& v2);

        void setParameter(const std::string& name, const glm::vec3& v3);

        void setParameter(const std::string& name, const glm::vec4& v4);

        void removeParameter(const std::string& name);

        const MaterialParameter& getParameter(const std::string& name) const;

        const std::map<std::string, MaterialParameter>& getParameters() const;

        void activate();

    private:
        ShaderPtr shader;
        std::map<std::string, TexturePtr> textures;
        std::map<std::string, MaterialParameter> parameters;
    };

    typedef SharedPtr<Material> MaterialPtr;
}