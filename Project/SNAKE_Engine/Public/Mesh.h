#pragma once
#include <vector>

typedef unsigned int GLuint;
typedef int GLsizei;

class Mesh {
public:
    Mesh(const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices = {});

    void Draw() const;

    ~Mesh();
private:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    GLsizei indexCount = 0;
    bool useIndex = false;

    void SetupMesh(const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices);
};