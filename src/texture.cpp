#include "texture.h"

#include <stb_image.h>

#include <iostream>

namespace wake
{
    Texture* Texture::load(const char* path)
    {
        int width;
        int height;
        int comp;
        unsigned char* data = stbi_load(path, &width, &height, &comp, STBI_rgb_alpha);
        if (data == nullptr)
        {
            std::cout << "Texture::load error: there was a problem loading the texture " << path << std::endl;
            return nullptr;
        }

        return new Texture(data, width, height, comp);
    }

    Texture::Texture(unsigned char* data, int width, int height, int comp)
    {
        this->data = data;
        this->width = width;
        this->height = height;
        this->comp = comp;
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
}