#include "material.h"

namespace wake
{
    ShaderPtr Material::getShader() const
    {
        return shader;
    }

    const std::unordered_map<std::string, TexturePtr>& Material::getTextures() const
    {
        return textures;
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