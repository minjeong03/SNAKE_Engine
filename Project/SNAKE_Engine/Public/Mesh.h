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

struct Vertex
{
    glm::vec3 position;
    glm::vec2 uv;
};

class Mesh {
    friend Material;
    friend RenderManager;

public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices = {}, PrimitiveType primitiveType = PrimitiveType::Triangles);

    ~Mesh();

    [[nodiscard]] glm::vec2 GetLocalBoundsHalfSize() const { return localHalfSize; }

private:
    void BindVAO() const;

    void SetupInstanceAttributes(GLuint* instanceVBO) const;

    void Draw() const;

    void DrawInstanced(GLsizei instanceCount) const;

    void SetupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

    void ComputeLocalBounds(const std::vector<Vertex>& vertices)
    {
        if (vertices.empty())
        {
            localHalfSize = glm::vec2(0.5f);
            return;
        }
        else if (vertices.size()==1)
        {
            localHalfSize = glm::vec2( 0.0001f);
            return;
        }

        glm::vec2 minPos = vertices[0].position;
        glm::vec2 maxPos = vertices[1].position;

        for (const auto& v : vertices)
        {
            glm::vec2 pos = v.position;
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
