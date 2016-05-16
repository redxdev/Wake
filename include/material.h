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
    // TODO: Support for all matrix types
    // TODO: Don't use a union, having a single parameter always contain at least the size of a mat4 is incredibly
    //       inefficient.
    struct MaterialParameter
    {
        static MaterialParameter NullParameter;

        MaterialParameter()
        {
            type = Null;
            i = 0;
            uniform = Uniform();
        }

        void setUniform(Uniform& uniform);

        enum : uint8
        {
            Null = 0,

            Int = 1,
            UInt = 2,
            Float = 3,

            Vec2 = 4,
            Vec3 = 5,
            Vec4 = 6,

            Mat4 = 7
        } type;

        union
        {
            GLint i;
            GLuint u;
            GLfloat f;

            glm::vec2 v2;
            glm::vec3 v3;
            glm::vec4 v4;

            glm::mat4 m4;
        };

        Uniform uniform;
    };

    struct MaterialTexParameter
    {
        MaterialTexParameter()
        {
            texture = nullptr;
            uniform = Uniform();
        }

        TexturePtr texture;
        Uniform uniform;
    };

    typedef SharedPtr<class Material> MaterialPtr;

    class Material
    {
    public:
        // Global material has parameters applied to all other materials.
        // Textures are not supported for the global material at this time.
        static MaterialPtr getGlobalMaterial();

    private:
        static MaterialPtr globalMaterial;

    public:
        Material();

        Material(const Material& other);

        ~Material();

        Material& operator=(const Material& other);

        const std::string& getTypeName() const;

        void setTypeName(const std::string& name);

        void setShader(ShaderPtr shader);

        ShaderPtr getShader() const;

        void setTexture(const std::string& name, TexturePtr texture);

        void removeTexture(const std::string& name);

        TexturePtr getTexture(const std::string& name);

        const std::map<std::string, MaterialTexParameter>& getTextures() const;

        size_t getTextureCount() const;

        void setParameter(const std::string& name, GLint i);

        void setParameter(const std::string& name, GLuint u);

        void setParameter(const std::string& name, GLfloat f);

        void setParameter(const std::string& name, const glm::vec2& v2);

        void setParameter(const std::string& name, const glm::vec3& v3);

        void setParameter(const std::string& name, const glm::vec4& v4);

        void setParameter(const std::string& name, const glm::mat4& m4);

        void removeParameter(const std::string& name);

        const MaterialParameter& getParameter(const std::string& name) const;

        const std::map<std::string, MaterialParameter>& getParameters() const;

        size_t getParameterCount() const;

        void copyFrom(MaterialPtr other);

        void use();

        void resetUniformCache();

    private:
        std::string typeName = "default";

        ShaderPtr shader;
        std::map<std::string, MaterialTexParameter> textures;
        std::map<std::string, MaterialParameter> parameters;

        std::map<std::string, Uniform> globalCache;

        bool needsUniformUpdates = false;
    };
}