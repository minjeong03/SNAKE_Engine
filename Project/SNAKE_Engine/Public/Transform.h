#pragma once
#include "../ThirdParty/glm/glm.hpp"
class Transform2D
{
public:
    Transform2D() :position(0.f), rotation(0.f), scale(0.f) {}
    glm::vec2 position;
    float rotation; // radians
    glm::vec2 scale;

    glm::mat4 GetMatrix() const;
};
