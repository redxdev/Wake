#include "material.h"

namespace wake
{
    MaterialParameter MaterialParameter::NullParameter = MaterialParameter();

    Material::Material()
    {
        shader = nullptr;
        needsUniformUpdates = true;
    }

    Material::Material(const Material& other)
    {
        typeName = other.typeName;
        shader = other.shader;
        textures = other.textures;
        parameters = other.parameters;
        needsUniformUpdates = true;
    }

    Material::~Material()
    {
    }

    Material& Material::operator=(const Material& other)
    {
        typeName = other.typeName;
        shader = other.shader;
        textures = other.textures;
        parameters = other.parameters;
        needsUniformUpdates = true;
        return *this;
    }

    const std::string& Material::getTypeName() const
    {
        return typeName;
    }

    void Material::setTypeName(const std::string& name)
    {
        typeName = name;
    }

    void Material::setShader(ShaderPtr shader)
    {
        this->shader = shader;
        needsUniformUpdates = true;
    }

    ShaderPtr Material::getShader() const
    {
        return shader;
    }

    void Material::setTexture(const std::string& name, TexturePtr texture)
    {
        MaterialTexParameter param;
        param.texture = texture;
        textures[name] = param;
    }

    void Material::removeTexture(const std::string& name)
    {
        textures.erase(name);
    }

    TexturePtr Material::getTexture(const std::string& name)
    {
        auto found = textures.find(name);
        if (found == textures.end())
            return nullptr;

        return found->second.texture;
    }

    const std::map<std::string, MaterialTexParameter>& Material::getTextures() const
    {
        return textures;
    }

    void Material::setParameter(const std::string& name, GLint i)
    {
        MaterialParameter param;
        param.type = MaterialParameter::Int;
        param.i = i;
        parameters[name] = param;
    }

    void Material::setParameter(const std::string& name, GLuint u)
    {
        MaterialParameter param;
        param.type = MaterialParameter::UInt;
        param.u = u;
        parameters[name] = param;
    }

    void Material::setParameter(const std::string& name, GLfloat f)
    {
        MaterialParameter param;
        param.type = MaterialParameter::Float;
        param.f = f;
        parameters[name] = param;
    }

    void Material::setParameter(const std::string& name, const glm::vec2& v2)
    {
        MaterialParameter param;
        param.type = MaterialParameter::Vec2;
        param.v2 = v2;
        parameters[name] = param;
    }

    void Material::setParameter(const std::string& name, const glm::vec3& v3)
    {
        MaterialParameter param;
        param.type = MaterialParameter::Vec3;
        param.v3 = v3;
        parameters[name] = param;
    }

    void Material::setParameter(const std::string& name, const glm::vec4& v4)
    {
        MaterialParameter param;
        param.type = MaterialParameter::Vec4;
        param.v4 = v4;
        parameters[name] = param;
    }

    void Material::removeParameter(const std::string& name)
    {
        parameters.erase(name);
    }

    const MaterialParameter& Material::getParameter(const std::string& name) const
    {
        auto found = parameters.find(name);
        if (found == parameters.end())
            return MaterialParameter::NullParameter;
        return found->second;
    }

    const std::map<std::string, MaterialParameter>& Material::getParameters() const
    {
        return parameters;
    }

    void Material::copyFrom(wake::MaterialPtr other)
    {
        if (shader.get() == nullptr)
        {
            shader = other->getShader();
        }

        for (auto& param : other->getTextures())
        {
            auto ours = getTexture(param.first);
            if (ours.get() == nullptr)
            {
                setTexture(param.first, param.second.texture);
            }
        }

        for (auto& param : other->getParameters())
        {
            auto& ours = getParameter(param.first);
            if (ours.type == MaterialParameter::Null)
            {
                parameters[param.first] = param.second;
            }
        }
    }

    void Material::use()
    {
        if (shader.get() == nullptr)
            return;
        
        shader->use();

        uint32 texUnit = 0;
        for (auto entry : textures)
        {
            const std::string& name = entry.first;
            TexturePtr texture = entry.second.texture;
            if (texture.get() == nullptr)
                continue;

            if (needsUniformUpdates || entry.second.uniform.isError())
            {
                entry.second.uniform = shader->getUniform(name.data());
            }

            Uniform& uniform = entry.second.uniform;
            if (uniform.isError())
                continue;

            texture->activate(texUnit);
            uniform.set1i(texUnit);
            ++texUnit;
        }

        for (auto entry : parameters)
        {
            const std::string& name = entry.first;
            MaterialParameter& param = entry.second;

            if (needsUniformUpdates || param.uniform.isError())
            {
                param.uniform = shader->getUniform(name.data());
            }

            Uniform& uniform = param.uniform;
            if (uniform.isError())
                continue;

            switch (param.type)
            {
                default:
                case MaterialParameter::Null:
                    continue;

                case MaterialParameter::Int:
                    uniform.set1i(param.i);
                    break;

                case MaterialParameter::UInt:
                    uniform.set1ui(param.u);
                    break;

                case MaterialParameter::Float:
                    uniform.set1f(param.f);
                    break;

                case MaterialParameter::Vec2:
                    uniform.setVec2(param.v2);
                    break;

                case MaterialParameter::Vec3:
                    uniform.setVec3(param.v3);
                    break;

                case MaterialParameter::Vec4:
                    uniform.setVec4(param.v4);
                    break;
            }
        }

        needsUniformUpdates = false;
    }

    void Material::resetUniformCache()
    {
        needsUniformUpdates = true;
    }
}