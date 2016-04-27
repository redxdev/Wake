#include "mesh.h"
#include <iostream>

namespace wake
{
    Mesh::Mesh()
    {
        initializeData();

        updateVertexBuffer();
        updateElementBuffer();
    }

    Mesh::Mesh(const std::vector<Vertex>& vertices)
    {
        initializeData();

        this->vertices = vertices;

        indices.resize(vertices.size());
        for (GLuint i = 0; i < vertices.size(); ++i)
        {
            indices[i] = i;
        }

        updateVertexBuffer();
        updateElementBuffer();
    }

    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
    {
        initializeData();

        this->vertices = vertices;
        this->indices = indices;

        updateVertexBuffer();
        updateElementBuffer();
    }

    Mesh::Mesh(const Mesh& other)
    {
        initializeData();

        vertices = other.vertices;
        indices = other.indices;

        updateVertexBuffer();
        updateElementBuffer();
    }

    Mesh::~Mesh()
    {
        if (vao != 0)
        {
            glDeleteVertexArrays(1, &vao);
            vao = 0;
        }

        if (vbo != 0)
        {
            glDeleteBuffers(1, &vbo);
            vbo = 0;
        }

        if (ebo != 0)
        {
            glDeleteBuffers(1, &ebo);
            ebo = 0;
        }
    }

    Mesh& Mesh::operator=(const Mesh& other)
    {
        initializeData();

        vertices = other.vertices;
        indices = other.indices;

        updateVertexBuffer();
        updateElementBuffer();

        return *this;
    }

    const std::vector<Vertex>& Mesh::getVertices() const
    {
        return vertices;
    }

    void Mesh::setVertices(const std::vector<Vertex>& vertices, bool updateIndices)
    {
        this->vertices = vertices;

        updateVertexBuffer();

        if (updateIndices)
        {
            indices.resize(vertices.size());
            for (GLuint i = 0; i < vertices.size(); ++i)
            {
                indices[i] = i;
            }

            updateElementBuffer();
        }
    }

    const std::vector<GLuint>& Mesh::getIndices() const
    {
        return indices;
    }

    void Mesh::setIndices(const std::vector<GLuint>& indices)
    {
        this->indices = indices;

        updateElementBuffer();
    }

    void Mesh::draw()
    {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        W_GL_CHECK();
    }

    void Mesh::initializeData()
    {
        if (vao != 0 || vbo != 0 || ebo != 0)
        {
            std::cout <<
            "Mesh::initializeData() was called when vao/vbo/ebo were already initialized, not re-initializing" <<
            std::endl;
            return;
        }

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        W_GL_CHECK();

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindVertexArray(vao);
        W_GL_CHECK();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, texCoords));

        W_GL_CHECK();

        glBindVertexArray(0);
        W_GL_CHECK();
    }

    void Mesh::updateVertexBuffer()
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);
        W_GL_CHECK();
    }

    void Mesh::updateElementBuffer()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);
        W_GL_CHECK();
    }
}