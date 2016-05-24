#pragma once

#include "glutil.h"
#include "engineptr.h"

namespace wake
{
    class Texture;

    typedef SharedPtr<Texture> TexturePtr;

    // RGBA texture
    class Texture
    {
    public:
        static TexturePtr load(const char* path);

    public:
        Texture(unsigned char* data, int width, int height, int comp, const std::string& path);

        Texture();

        Texture(const Texture& other);

        ~Texture();

        Texture& operator=(const Texture& other);

        unsigned char* getData() const;

        int getWidth() const;

        int getHeight() const;

        int getComponentsPerPixel() const;

        const std::string& getPath() const;

        void bind();

        void generateMipMaps();

        void activate(GLenum unit);

    private:
        void initializeData();

        unsigned char* data;
        int width;
        int height;
        int comp;
        std::string path;

        GLuint texture = 0;
    };
}