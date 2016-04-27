#include "mesh.h"
#include <iostream>

namespace wake
{
    Mesh::Mesh(bool initialize)
    {
        initializeData();

        needsDataUpdate = true;

        if (initialize)
        {
            updateGLData(true);
        }
    }

    Mesh::Mesh(const std::vector<Vertex>& newVertices, bool initialize)
    {
        initializeData();

        needsDataUpdate = true;

        vertices = newVertices;

        if (initialize)
        {
            updateGLData(true);
        }
    }

    Mesh::Mesh(const Mesh& other)
    {
        initializeData();

        needsDataUpdate = true;

        vertices = other.vertices;

        if (!other.needsDataUpdate)
        {
            updateGLData(true);
        }
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
    }

    Mesh& Mesh::operator=(const Mesh& other)
    {
        initializeData();

        needsDataUpdate = true;

        vertices = other.vertices;

        if (!other.needsDataUpdate)
        {
            updateGLData(true);
        }

        return *this;
    }

    void Mesh::setVertices(const std::vector<Vertex>& newVertices, bool updateImmediately)
    {
        vertices = newVertices;
        needsDataUpdate = true;

        if (updateImmediately)
            updateGLData();
    }

    const std::vector<Vertex>& Mesh::getVertices() const
    {
        return vertices;
    }

    void Mesh::draw()
    {
        if (needsDataUpdate)
        {
            updateGLData();
        }

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        W_GL_CHECK();
    }

    void Mesh::initializeData()
    {
        if (vao != 0 || vbo != 0)
        {
            std::cout << "Mesh::initializeData() was called when vao/vbo were already initialized, not re-initializing" << std::endl;
            return;
        }

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        W_GL_CHECK();

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindVertexArray(vao);
        W_GL_CHECK();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

        W_GL_CHECK();

        glBindVertexArray(0);
        W_GL_CHECK();
    }

    void Mesh::updateGLData(bool force)
    {
        if (!force && !needsDataUpdate)
            return;

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        W_GL_CHECK();

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);
        W_GL_CHECK();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        W_GL_CHECK();

        needsDataUpdate = false;
    }
}