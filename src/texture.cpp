#include "texture.h"
#include "wake.h"

#include <stb_image.h>

#include <iostream>
#include <cstdlib>
#include <cstring>

namespace wake
{
    TexturePtr Texture::load(const char* path)
    {
        int width;
        int height;
        int comp;
        stbi_set_flip_vertically_on_load(1); // we need to flip the y axis due to OpenGL
        unsigned char* data = stbi_load(path, &width, &height, &comp, STBI_rgb_alpha);
        if (data == nullptr)
        {
            std::cout << "Texture::load error: there was a problem loading the texture " << path << std::endl;
            std::cout << "STBI: " << stbi_failure_reason() << std::endl;
            return nullptr;
        }

        return TexturePtr(new Texture(data, width, height, comp, path));
    }

    Texture::Texture()
    {
        data = nullptr;
        width = 0;
        height = 0;
        comp = 0;

        initializeData();
    }

    Texture::Texture(unsigned char* data, int width, int height, int comp, const std::string& path)
    {
        this->data = data;
        this->width = width;
        this->height = height;
        this->comp = comp;
        this->path = path;

        initializeData();
    }

    Texture::Texture(const Texture& other)
    {
        data = (unsigned char*) malloc(sizeof(unsigned char) * other.width * other.height * other.comp);
        memcpy(data, other.data, sizeof(unsigned char) * other.width * other.height * other.comp);
        width = other.width;
        height = other.height;
        comp = other.comp;

        initializeData();
    }

    Texture::~Texture()
    {
        free(data);

        if (texture != 0)
        {
            glDeleteTextures(1, &texture);
            texture = 0;
        }
    }

    Texture& Texture::operator=(const Texture& other)
    {
        data = (unsigned char*) malloc(sizeof(unsigned char) * other.width * other.height * other.comp);
        memcpy(data, other.data, sizeof(unsigned char) * other.width * other.height * other.comp);
        width = other.width;
        height = other.height;
        comp = other.comp;

        initializeData();

        return *this;
    }

    unsigned char* Texture::getData() const
    {
        return data;
    }

    int Texture::getWidth() const
    {
        return width;
    }

    int Texture::getHeight() const
    {
        return height;
    }

    int Texture::getComponentsPerPixel() const
    {
        return comp;
    }

    const std::string& Texture::getPath() const
    {
        return path;
    }

    void Texture::bind()
    {
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    void Texture::generateMipMaps()
    {
        bind();
        glGenerateMipmap(GL_TEXTURE_2D);

        W_GL_CHECK();
    }

    void Texture::activate(GLenum unit)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        bind();

        W_GL_CHECK();
    }

    void Texture::initializeData()
    {
        if (getEngineMode() != EngineMode::Normal)
        {
            return;
        }

        if (texture != 0)
        {
            std::cout <<
            "Texture::initializeData was called when texture was already initialized, not re-initializing" <<
            std::endl;
            return;
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        W_GL_CHECK();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        W_GL_CHECK();

        if (data)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        W_GL_CHECK();
    }
}