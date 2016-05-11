#include "material.h"

namespace wake
{
    ShaderPtr Material::getShader() const
    {
        return shader;
    }

    void Material::setTexture(const std::string& name, TexturePtr texture)
    {
        textures[name] = texture;
    }

    const std::unordered_map<std::string, TexturePtr>& Material::getTextures() const
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

    const std::unordered_map<std::string, MaterialParameter> Material::getParameters() const
    {
        return parameters;
    }

    void Material::activate()
    {
        shader->use();

        uint32 texUnit = 0;
        for (auto entry : textures)
        {
            const std::string& name = entry.first;
            TexturePtr texture = entry.second;

            Uniform uniform = shader->getUniform(name.data());
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

            Uniform uniform = shader->getUniform(name.data());
            if (uniform.isError())
                continue;

            switch (param.type)
            {
                default:
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
    }
}