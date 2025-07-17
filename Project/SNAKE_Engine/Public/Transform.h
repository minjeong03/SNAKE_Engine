#pragma once
#include "../ThirdParty/glm/glm.hpp"

class Transform2D
{
public:
    Transform2D()
        : position(0.f), rotation(0.f), scale(1.f),
        matrix(1.f), isChanged(true)
    {
    }

    void SetPosition(const glm::vec2& pos)
    {
        position = pos;
        isChanged = true;
    }

    void AddPosition(const glm::vec2& pos)
    {
        position += pos;
        isChanged = true;
    }

    void SetRotation(float rot)
    {
        rotation = rot;
        isChanged = true;
    }

    void SetScale(const glm::vec2& scl)
    {
        scale = scl;
        isChanged = true;
    }

    const glm::vec2& GetPosition() const { return position; }

    float GetRotation() const { return rotation; }

    const glm::vec2& GetScale() const { return scale; }

    glm::mat4& GetMatrix();

private:
    glm::vec2 position;
    float rotation;
    glm::vec2 scale;

    glm::mat4 matrix;
    bool isChanged;
};
