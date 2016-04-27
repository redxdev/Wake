#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "glutil.h"

namespace wake
{
    struct Vertex
    {
        Vertex()
                : position(0, 0, 0), normal(1, 0, 0), texCoords(0, 0)
        {
        }

        Vertex(const glm::vec3& position)
                : position(position), normal(1, 0, 0), texCoords(0, 0)
        {
        }

        Vertex(const glm::vec3& position, const glm::vec3& normal)
                : position(position), normal(normal), texCoords(0, 0)
        {
        }

        Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords)
                : position(position), normal(normal), texCoords(texCoords)
        {
        }

        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    class Mesh
    {
    public:
        // If initialize is false, the mesh data will not be sent to OpenGL immediately.
        Mesh();

        Mesh(const std::vector<Vertex>& vertices);

        Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);

        // This will send data to the graphics card immediately if and
        // only if the other mesh doesn't need to be updated as well.
        Mesh(const Mesh& other);

        ~Mesh();

        Mesh& operator=(const Mesh& other);

        const std::vector<Vertex>& getVertices() const;

        void setVertices(const std::vector<Vertex>& vertices, bool updateIndices = false);

        const std::vector<GLuint>& getIndices() const;

        void setIndices(const std::vector<GLuint>& indices);

        void draw();

    private:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ebo = 0;

        void initializeData();

        void updateVertexBuffer();

        void updateElementBuffer();
    };
}