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
    uint32 readUInt32(std::istream& in)
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

    void writeUInt32(std::ostream& out, uint32 val)
    {
        out.write((char*) &val, sizeof(val));

        if (out.bad())
        {
            std::cout << "writeUInt32 error: unable to read from stream" << std::endl;
            throw std::exception();
        }
    }

    uint64 readUInt64(std::istream& in)
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

    void writeUInt64(std::ostream& out, uint64 val)
    {
        out.write((char*) &val, sizeof(val));

        if (out.bad())
        {
            std::cout << "writeUInt64 error: unable to read from stream" << std::endl;
            throw std::exception();
        }
    }

    float readFloat(std::istream& in)
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

    void writeFloat(std::ostream& out, float val)
    {
        out.write((char*) &val, sizeof(val));

        if (out.bad())
        {
            std::cout << "writeUInt32 error: unable to read from stream" << std::endl;
            throw std::exception();
        }
    }

    glm::vec2 readVec2(std::istream& in)
    {
        glm::vec2 val;
        val.x = readFloat(in);
        val.y = readFloat(in);
        return val;
    }

    void writeVec2(std::ostream& out, const glm::vec2& val)
    {
        writeFloat(out, val.x);
        writeFloat(out, val.y);
    }

    glm::vec3 readVec3(std::istream& in)
    {
        glm::vec3 val;
        val.x = readFloat(in);
        val.y = readFloat(in);
        val.z = readFloat(in);
        return val;
    }

    void writeVec3(std::ostream& out, const glm::vec3& val)
    {
        writeFloat(out, val.x);
        writeFloat(out, val.y);
        writeFloat(out, val.z);
    }

    glm::vec4 readVec4(std::istream& in)
    {
        glm::vec4 val;
        val.x = readFloat(in);
        val.y = readFloat(in);
        val.z = readFloat(in);
        val.w = readFloat(in);
        return val;
    }

    void writeVec4(std::ostream& out, const glm::vec4& val)
    {
        writeFloat(out, val.x);
        writeFloat(out, val.y);
        writeFloat(out, val.z);
        writeFloat(out, val.w);
    }

    bool saveWMDL(const char* path, ModelPtr model, bool compress)
    {
        std::stringstream data;

        try
        {
            writeUInt32(data, model->getComponentCount());
            for (auto& component : model->getComponents())
            {
                auto& mesh = component.mesh;
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
            return false;
        }

        char* code = new char[strlen(W_MDL_CODE) + 1];
        f.read(code, strlen(W_MDL_CODE));
        code[strlen(W_MDL_CODE)] = '\0';

        if (strcmp(code, W_MDL_CODE) != 0)
        {
            delete[] code;
            f.close();
            std::cout << "loadWMDL error: bad header, expected " << W_MDL_CODE << ", got " << code << std::endl;
            return false;
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
                return false;
            }

            if (version > W_MDL_MAX_VERSION)
            {
                std::cout << "loadWMDL error: version mismatch, version must be at most " << W_MDL_MAX_VERSION <<
                " but the file is version " << version << std::endl;
                f.close();
                return false;
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

            uint32 meshCount = readUInt32(data);
            for (uint32 m = 0; m < meshCount; ++m)
            {
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

                ModelComponent component;
                component.mesh = mesh;
                model->addComponent(component);
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