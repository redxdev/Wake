#include "wmdl.h"

#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>

#include <snappy.h>

// TODO: Make everything host endian independent
// TODO: Make everything loads/saves floats correctly in case of different implementation than IEEE-754 on the host
// TODO: Actual errors instead of just std::exception throws

namespace wake
{
    static uint8 readUInt8(std::istream& in)
    {
        uint8 val;
        in.read((char*) &val, sizeof(val));

        if (in.eof())
        {
            std::cout << "readUInt8 error: hit EOF while reading from stream" << std::endl;
            throw std::exception();
        }

        if (in.bad())
        {
            std::cout << "readUInt8 error: unable to read from stream" << std::endl;
            throw std::exception();
        }

        return val;
    }

    static void writeUInt8(std::ostream& out, uint32 val)
    {
        out.write((char*) &val, sizeof(val));

        if (out.bad())
        {
            std::cout << "writeUInt8 error: unable to write to stream" << std::endl;
            throw std::exception();
        }
    }

    static uint32 readUInt32(std::istream& in)
    {
        uint32 val;
        in.read((char*) &val, sizeof(val));

        if (in.eof())
        {
            std::cout << "readUInt32 error: hit EOF while reading from stream" << std::endl;
            throw std::exception();
        }

        if (in.bad())
        {
            std::cout << "readUInt32 error: unable to read from stream" << std::endl;
            throw std::exception();
        }

        return val;
    }

    static void writeUInt32(std::ostream& out, uint32 val)
    {
        out.write((char*) &val, sizeof(val));

        if (out.bad())
        {
            std::cout << "writeUInt32 error: unable to write to stream" << std::endl;
            throw std::exception();
        }
    }

    static int32 readInt32(std::istream& in)
    {
        int32 val;
        in.read((char*) &val, sizeof(val));

        if (in.eof())
        {
            std::cout << "readInt32 error: hit EOF while reading from stream" << std::endl;
            throw std::exception();
        }

        if (in.bad())
        {
            std::cout << "readInt32 error: unable to read from stream" << std::endl;
            throw std::exception();
        }

        return val;
    }

    static void writeInt32(std::ostream& out, int32 val)
    {
        out.write((char*) &val, sizeof(val));

        if (out.bad())
        {
            std::cout << "writeInt32 error: unable to write to stream" << std::endl;
            throw std::exception();
        }
    }

    static uint64 readUInt64(std::istream& in)
    {
        uint64 val;
        in.read((char*) &val, sizeof(val));

        if (in.eof())
        {
            std::cout << "readUInt64 error: hit EOF while reading from stream" << std::endl;
            throw std::exception();
        }

        if (in.bad())
        {
            std::cout << "readUInt64 error: unable to read from stream" << std::endl;
            throw std::exception();
        }

        return val;
    }

    static void writeUInt64(std::ostream& out, uint64 val)
    {
        out.write((char*) &val, sizeof(val));

        if (out.bad())
        {
            std::cout << "writeUInt64 error: unable to write to stream" << std::endl;
            throw std::exception();
        }
    }

    static float readFloat(std::istream& in)
    {
        float val;
        in.read((char*) &val, sizeof(val));

        if (in.eof())
        {
            std::cout << "readFloat error: hit EOF while reading from stream" << std::endl;
            throw std::exception();
        }

        if (in.bad())
        {
            std::cout << "readFloat error: unable to read from stream" << std::endl;
            throw std::exception();
        }

        return val;
    }

    static void writeFloat(std::ostream& out, float val)
    {
        out.write((char*) &val, sizeof(val));

        if (out.bad())
        {
            std::cout << "writeFloat error: unable to write to stream" << std::endl;
            throw std::exception();
        }
    }

    static std::string readString(std::istream& in)
    {
        uint32 len = readUInt32(in);
        char* data = new char[len + 1];
        data[len] = '\0';
        in.read(data, len);
        std::string result = data;
        delete[] data;
        return result;
    }

    static void writeString(std::ostream& out, const std::string& val)
    {
        writeUInt32(out, (uint32) val.size());
        const char* data = val.data();
        out.write(data, val.size());

        if (out.bad())
        {
            std::cout << "writeString error: unable to write to stream" << std::endl;
            throw std::exception();
        }
    }

    static glm::vec2 readVec2(std::istream& in)
    {
        glm::vec2 val;
        val.x = readFloat(in);
        val.y = readFloat(in);
        return val;
    }

    static void writeVec2(std::ostream& out, const glm::vec2& val)
    {
        writeFloat(out, val.x);
        writeFloat(out, val.y);
    }

    static glm::vec3 readVec3(std::istream& in)
    {
        glm::vec3 val;
        val.x = readFloat(in);
        val.y = readFloat(in);
        val.z = readFloat(in);
        return val;
    }

    static void writeVec3(std::ostream& out, const glm::vec3& val)
    {
        writeFloat(out, val.x);
        writeFloat(out, val.y);
        writeFloat(out, val.z);
    }

    static glm::vec4 readVec4(std::istream& in)
    {
        glm::vec4 val;
        val.x = readFloat(in);
        val.y = readFloat(in);
        val.z = readFloat(in);
        val.w = readFloat(in);
        return val;
    }

    static void writeVec4(std::ostream& out, const glm::vec4& val)
    {
        writeFloat(out, val.x);
        writeFloat(out, val.y);
        writeFloat(out, val.z);
        writeFloat(out, val.w);
    }

    static glm::mat4 readMatrix4(std::istream& in)
    {
        glm::mat4 val;
        val[0] = readVec4(in);
        val[1] = readVec4(in);
        val[2] = readVec4(in);
        val[3] = readVec4(in);
        return val;
    }

    static void writeMatrix4(std::ostream& out, const glm::mat4& val)
    {
        writeVec4(out, val[0]);
        writeVec4(out, val[1]);
        writeVec4(out, val[2]);
        writeVec4(out, val[3]);
    }

    bool saveWMDL(const char* path, ModelPtr model, bool compress)
    {
        std::stringstream data;

        try
        {
            ////
            // Material Section
            ////
            writeUInt32(data, (uint32) model->getMaterialCount());
            for (auto& matInfo : model->getMaterials())
            {
                writeString(data, matInfo.name);
                writeString(data, matInfo.material->getTypeName());

                // Textures
                auto& textures = matInfo.material->getTextures();
                writeUInt32(data, (uint32) textures.size());
                for (auto& texEntry : textures)
                {
                    writeString(data, texEntry.first);
                    if (texEntry.second.texture.get() == nullptr)
                        writeString(data, "");
                    else
                        writeString(data, texEntry.second.texture->getPath());
                }

                // Parameters
                auto& params = matInfo.material->getParameters();
                writeUInt32(data, (uint32) params.size());
                for (auto& paramEntry : params)
                {
                    writeString(data, paramEntry.first);
                    writeUInt8(data, (uint8) paramEntry.second.type);
                    switch (paramEntry.second.type)
                    {
                        default:
                            std::cout << "Unable to write material " << matInfo.name << ": unknown parameter type " <<
                            paramEntry.second.type << " for parameter " << paramEntry.first << std::endl;
                            throw std::exception();

                        case MaterialParameter::Null:
                            break;

                        case MaterialParameter::Int:
                            writeInt32(data, paramEntry.second.i);
                            break;

                        case MaterialParameter::UInt:
                            writeUInt32(data, paramEntry.second.u);
                            break;

                        case MaterialParameter::Float:
                            writeFloat(data, paramEntry.second.f);
                            break;

                        case MaterialParameter::Vec2:
                            writeVec2(data, paramEntry.second.v2);
                            break;

                        case MaterialParameter::Vec3:
                            writeVec3(data, paramEntry.second.v3);
                            break;

                        case MaterialParameter::Vec4:
                            writeVec4(data, paramEntry.second.v4);
                            break;

                        case MaterialParameter::Mat4:
                            writeMatrix4(data, paramEntry.second.m4);
                            break;
                    }
                }
            }

            ////
            // Model Section
            ////

            writeUInt32(data, (uint32) model->getMeshCount());
            for (auto& meshInfo : model->getMeshes())
            {
                writeInt32(data, meshInfo.materialIndex);

                auto& mesh = meshInfo.mesh;
                auto& vertices = mesh->getVertices();
                writeUInt32(data, vertices.size());
                for (auto& vertex : vertices)
                {
                    writeVec3(data, vertex.position);
                    writeVec3(data, vertex.normal);
                    writeVec2(data, vertex.texCoords);
                }

                auto& indices = mesh->getIndices();
                writeUInt32(data, indices.size());
                for (GLuint index : indices)
                {
                    writeUInt32(data, index);
                }
            }
        }
        catch (std::exception& e)
        {
            return false;
        }

        std::string dataStr = data.str();

        std::fstream f(path, std::ios::out | std::ios::ate | std::ios::binary);
        try
        {
            if (!f.is_open())
            {
                std::cout << "saveWMDL error: unable to open file \"" << path << "\" for writing." << std::endl;
                return false;
            }

            ////
            // Header
            ////

            uint64 flags = W_MDL_FLAG_NONE;
            if (compress)
            {
                std::string result;
                snappy::Compress(dataStr.c_str(), dataStr.length(), &result);

                f.write(W_MDL_CODE, strlen(W_MDL_CODE));
                writeUInt32(f, W_MDL_VERSION);
                flags |= W_MDL_FLAG_COMPRESS;
                writeUInt64(f, flags);

                f.write(result.c_str(), result.size());
            }
            else
            {
                f.write(W_MDL_CODE, strlen(W_MDL_CODE));
                writeUInt32(f, W_MDL_VERSION);
                writeUInt64(f, flags);

                f.write(dataStr.c_str(), dataStr.length());
            }

            if (!f.good())
            {
                std::cout << "saveWMDL error: unable to write to output file \"" << path << "\"" << std::endl;
                return false;
            }

            f.close();
            return true;
        }
        catch (std::exception& e)
        {
            if (f.is_open())
            {
                f.close();
            }

            return false;
        }
    }

    ModelPtr loadWMDL(const char* path)
    {
        std::fstream f(path, std::ios::in | std::ios::binary);
        if (!f.is_open())
        {
            std::cout << "loadWMDL error: unable to open file \"" << path << "\" for reading." << std::endl;
            return ModelPtr(nullptr);
        }

        char* code = new char[strlen(W_MDL_CODE) + 1];
        f.read(code, strlen(W_MDL_CODE));
        code[strlen(W_MDL_CODE)] = '\0';

        if (strcmp(code, W_MDL_CODE) != 0)
        {
            delete[] code;
            f.close();
            std::cout << "loadWMDL error: bad header, expected " << W_MDL_CODE << ", got " << code << std::endl;
            return ModelPtr(nullptr);
        }

        delete[] code;

        ModelPtr model(new Model());

        try
        {
            uint32 version = readUInt32(f);
            if (version < W_MDL_MIN_VERSION)
            {
                std::cout << "loadWMDL error: version mismatch, version must be at least " << W_MDL_MIN_VERSION <<
                " but the file is version " << version << std::endl;
                f.close();
                return ModelPtr(nullptr);
            }

            if (version > W_MDL_MAX_VERSION)
            {
                std::cout << "loadWMDL error: version mismatch, version must be at most " << W_MDL_MAX_VERSION <<
                " but the file is version " << version << std::endl;
                f.close();
                return ModelPtr(nullptr);
            }

            uint64 flags = readUInt64(f);
            std::string raw((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
            f.close();

            if (flags & W_MDL_FLAG_COMPRESS)
            {
                std::string uncompressed;
                snappy::Uncompress(raw.c_str(), raw.length(), &uncompressed);
                raw = uncompressed;
            }

            std::stringstream data(raw);

            // Material Section
            // Only valid in version 4+
            if (version >= 4)
            {
                uint32 materialCount = readUInt32(data);
                for (uint32 k = 0; k < materialCount; ++k)
                {
                    std::string matName = readString(data);
                    MaterialPtr mat(new Material());

                    if (version >= 5)
                    {
                        std::string matType = readString(data);
                        mat->setTypeName(matType);
                    }

                    uint32 textureCount = readUInt32(data);
                    for (uint32 t = 0; t < textureCount; ++t)
                    {
                        TexturePtr texture(nullptr);
                        std::string textureName = readString(data);

                        if (version >= 6)
                        {
                            std::string texturePath = readString(data);
                            if (texturePath != "")
                                texture = Texture::load(texturePath.data());
                        }

                        mat->setTexture(textureName, texture);
                    }

                    uint32 paramCount = readUInt32(data);
                    for (uint32 p = 0; p < paramCount; ++p)
                    {
                        std::string paramName = readString(data);
                        uint8 type = readUInt8(data);
                        switch (type)
                        {
                            default:
                                std::cout << "Unable to read material " << matName << ": unknown parameter type " <<
                                type << " for parameter " << paramName << std::endl;
                                throw std::exception();

                            case MaterialParameter::Null:
                                break;

                            case MaterialParameter::Int:
                                mat->setParameter(paramName, readInt32(data));
                                break;

                            case MaterialParameter::UInt:
                                mat->setParameter(paramName, readUInt32(data));
                                break;

                            case MaterialParameter::Float:
                                mat->setParameter(paramName, readFloat(data));
                                break;

                            case MaterialParameter::Vec2:
                                mat->setParameter(paramName, readVec2(data));
                                break;

                            case MaterialParameter::Vec3:
                                mat->setParameter(paramName, readVec3(data));
                                break;

                            case MaterialParameter::Vec4:
                                mat->setParameter(paramName, readVec4(data));
                                break;

                            case MaterialParameter::Mat4:
                                mat->setParameter(paramName, readMatrix4(data));
                                break;
                        }
                    }

                    model->addMaterial(matName, mat);
                }
            }

            // Model Section
            // Valid in all versions
            uint32 meshCount = readUInt32(data);
            for (uint32 m = 0; m < meshCount; ++m)
            {
                int32 materialIndex = readInt32(data);

                std::vector<Vertex> vertices;
                uint32 vertexCount = readUInt32(data);
                vertices.reserve(vertexCount);
                for (uint32 v = 0; v < vertexCount; ++v)
                {
                    Vertex vertex;
                    vertex.position = readVec3(data);
                    vertex.normal = readVec3(data);
                    vertex.texCoords = readVec2(data);

                    vertices.push_back(vertex);
                }

                std::vector<GLuint> indices;
                uint32 indexCount = readUInt32(data);
                indices.reserve(indexCount);
                for (uint32 i = 0; i < indexCount; ++i)
                {
                    indices.push_back(readUInt32(data));
                }

                MeshPtr mesh = MeshPtr(new Mesh(vertices, indices));
                model->addMesh(mesh, materialIndex);

                ModelMetadata metadata;
                metadata.source = ModelMetadata::WMDL;
                metadata.version = version;
                metadata.path = path;

                model->setMetadata(metadata);
            }
        }
        catch (std::exception& e)
        {
            if (f.is_open())
            {
                f.close();
            }

            return ModelPtr(nullptr);
        }

        return model;
    }
}