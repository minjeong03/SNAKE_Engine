#pragma once
#include "../ThirdParty/glm/glm.hpp"
class Transform2D
{
public:
    glm::vec2 position{ 0.0f };
    float rotation = 0.0f; // radians
    glm::vec2 scale{ 1.0f };

    glm::mat4 GetMatrix() const;
};
