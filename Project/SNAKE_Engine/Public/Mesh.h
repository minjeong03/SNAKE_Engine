#pragma once
#include <vector>
#include "Material.h"

class ObjectManager;

using GLuint = unsigned int;
using GLsizei = int;

enum class PrimitiveType
{
    Triangles,
    Lines,
    Points,
    TriangleFan,
    TriangleStrip,
    LineStrip
};
class Mesh {
    friend Material;
    friend RenderManager;

public:
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices = {}, PrimitiveType primitiveType = PrimitiveType::Triangles);

    ~Mesh();

    glm::vec2 GetLocalBoundsHalfSize() const { return localHalfSize; }

private:
    void BindVAO() const;

    void SetupInstanceAttributes(GLuint* instanceVBO) const;

    void Draw() const;

    void DrawInstanced(GLsizei instanceCount) const;

    void SetupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    void ComputeLocalBounds(const std::vector<float>& vertices)
    {
        if (vertices.empty() || vertices.size() % 3 != 0)
        {
            localHalfSize = glm::vec2(0.5f);
            return;
        }

        glm::vec2 minPos(vertices[0], vertices[1]);
        glm::vec2 maxPos(vertices[0], vertices[1]);

        for (size_t i = 0; i < vertices.size(); i += 3)
        {
            glm::vec2 pos(vertices[i], vertices[i + 1]);

            minPos = glm::min(minPos, pos);
            maxPos = glm::max(maxPos, pos);
        }

        glm::vec2 size = maxPos - minPos;
        localHalfSize = size * 0.5f;
    }

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLsizei indexCount;
    bool useIndex;

    PrimitiveType primitiveType;
    glm::vec2 localHalfSize;
};
