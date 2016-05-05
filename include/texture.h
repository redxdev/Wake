#pragma once

namespace wake
{
    // RGBA texture
    class Texture
    {
    public:
        static Texture* load(const char* path);

    public:
        Texture(unsigned char* data, int width, int height, int comp);

        unsigned char* getData() const;

        int getWidth() const;

        int getHeight() const;

        int getComponentsPerPixel() const;

    private:
        unsigned char* data;
        int width;
        int height;
        int comp;
    };
}