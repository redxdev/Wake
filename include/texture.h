#pragma once

#include "glutil.h"

namespace wake
{
    // RGBA texture
    class Texture
    {
    public:
        static Texture* load(const char* path);

    public:
        Texture(unsigned char* data, int width, int height, int comp);

        Texture(const Texture& other);

        ~Texture();

        Texture& operator=(const Texture& other);

        unsigned char* getData() const;

        int getWidth() const;

        int getHeight() const;

        int getComponentsPerPixel() const;

        void bind();

        void enableMipMaps();

        void activate(GLuint unit);

    private:
        void initializeData();

        unsigned char* data;
        int width;
        int height;
        int comp;

        GLuint texture = 0;
    };
}