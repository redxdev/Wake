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
    uint32_t readUInt32(std::istream& in)
    {
        uint32_t val;
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

    void writeUInt32(std::ostream& out, uint32_t val)
    {
        out.write((char*) &val, sizeof(val));

        if (out.bad())
        {
            std::cout << "writeUInt32 error: unable to read from stream" << std::endl;
            throw std::exception();
        }
    }

    uint64_t readUInt64(std::istream& in)
    {
        uint64_t val;
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

    void writeUInt64(std::ostream& out, uint64_t val)
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

    bool saveWMDL(const char* path, const std::vector<Mesh*>& meshes, bool compress)
    {
        std::stringstream data;

        try
        {
            writeUInt32(data, meshes.size());
            for (auto* mesh : meshes)
            {
                auto& vertices = mesh->getVertices();
                writeUInt32(data, vertices.size());
                for (auto& vertex : vertices)
                {
                    writeFloat(data, vertex.position.x);
                    writeFloat(data, vertex.position.y);
                    writeFloat(data, vertex.position.z);

                    writeFloat(data, vertex.normal.x);
                    writeFloat(data, vertex.normal.y);
                    writeFloat(data, vertex.normal.z);

                    writeFloat(data, vertex.texCoords.x);
                    writeFloat(data, vertex.texCoords.y);
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

            uint64_t flags = W_MDL_FLAG_NONE;
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
        catch(std::exception& e)
        {
            if (f.is_open())
            {
                f.close();
            }

            return false;
        }
    }

    bool loadWMDL(const char* path, std::vector<Mesh*>& meshes)
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

        try
        {
            uint32_t version = readUInt32(f);
            if (version != W_MDL_VERSION)
            {
                std::cout << "loadWMDL error: version mismatch, expected " << W_MDL_VERSION << ", got " << version << std::endl;
                f.close();
                return false;
            }

            // no need to check bitflags yet, we only have one so we can do simple comparison
            uint64_t flags = readUInt64(f);

            std::string raw((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
            f.close();

            if (flags & W_MDL_FLAG_COMPRESS)
            {
                std::string uncompressed;
                snappy::Uncompress(raw.c_str(), raw.length(), &uncompressed);
                raw = uncompressed;
            }

            std::stringstream data(raw);

            uint32_t meshCount = readUInt32(data);
            meshes.reserve(meshCount + meshes.size());
            for (uint32_t m = 0; m < meshCount; ++m)
            {
                std::vector<Vertex> vertices;
                uint32_t vertexCount = readUInt32(data);
                vertices.reserve(vertexCount);
                for (uint32_t v = 0; v < vertexCount; ++v)
                {
                    Vertex vertex;

                    vertex.position.x = readFloat(data);
                    vertex.position.y = readFloat(data);
                    vertex.position.z = readFloat(data);

                    vertex.normal.x = readFloat(data);
                    vertex.normal.y = readFloat(data);
                    vertex.normal.z = readFloat(data);

                    vertex.texCoords.x = readFloat(data);
                    vertex.texCoords.y = readFloat(data);

                    vertices.push_back(vertex);
                }

                std::vector<GLuint> indices;
                uint32_t indexCount = readUInt32(data);
                indices.reserve(indexCount);
                for (uint32_t i = 0; i < indexCount; ++i)
                {
                    indices.push_back(readUInt32(data));
                }

                Mesh* mesh = new Mesh(vertices, indices);
                meshes.push_back(mesh);
            }
        }
        catch (std::exception& e)
        {
            if (f.is_open())
            {
                f.close();
            }

            return false;
        }

        return true;
    }
}