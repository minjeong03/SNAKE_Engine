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

Mesh::~Mesh()
{
    if (ebo) glDeleteBuffers(1, &ebo);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (vao) glDeleteVertexArrays(1, &vao);
}


void Mesh::SetupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
    useIndex = !indices.empty();
    indexCount = useIndex ? static_cast<GLsizei>(indices.size()) : static_cast<GLsizei>(vertices.size() / 3);

    // Create VAO
    glCreateVertexArrays(1, &vao);

    // Create and bind VBO
    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Bind VBO to VAO
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, 3 * sizeof(float));

    glEnableVertexArrayAttrib(vao, 0); // position
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 0, 0);

    glEnableVertexArrayAttrib(vao, 1); // uv
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3);
    glVertexArrayAttribBinding(vao, 1, 0);

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float) * 5);

    // EBO (Element Buffer)
    if (useIndex)
    {
        glCreateBuffers(1, &ebo);
        glNamedBufferData(ebo, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glVertexArrayElementBuffer(vao, ebo);
    }
}
