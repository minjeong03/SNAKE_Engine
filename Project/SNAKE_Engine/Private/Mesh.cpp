#include "Mesh.h"
#include <glad/gl.h>

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
    SetupMesh(vertices, indices);
}

void Mesh::Draw() const
{
    glBindVertexArray(vao);

    if (useIndex)
    {
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, indexCount);
    }
}

void Mesh::SetupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
    useIndex = !indices.empty();
    indexCount = useIndex ? static_cast<GLsizei>(indices.size()) : static_cast<GLsizei>(vertices.size() / 3);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // EBO
    if (useIndex)
    {
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);
    }

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}
