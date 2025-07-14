#pragma once
#include <vector>
#include "Material.h"


class ObjectManager;

typedef unsigned int GLuint;
typedef int GLsizei;
/**
 * @brief
 * @details
 * @author Jinwoo Choi
 * @date 2025-07-13
 */

class Mesh {
    friend Material;
    friend ObjectManager;
public:
    Mesh(const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices = {});

    void Draw() const;

    void DrawInstanced(GLsizei instanceCount) const;

    void BindVAO() const;

    ~Mesh();

protected:
    void SetupInstanceAttributes(GLuint instanceVBO) const;

private:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    GLsizei indexCount = 0;
    bool useIndex = false;

    void SetupMesh(const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices);
};