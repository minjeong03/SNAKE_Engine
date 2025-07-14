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
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices = {});

    ~Mesh();

private:
    void BindVAO() const;
    void SetupInstanceAttributes(GLuint instanceVBO) const;
    void Draw() const;
    void DrawInstanced(GLsizei instanceCount) const;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLsizei indexCount;
    bool useIndex;

    void SetupMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
};