#pragma once
#include "../ThirdParty/glm/glm.hpp"

class Camera2D
{
public:
    Camera2D(int screenWidth = 800, int screenHeight = 600);

    void SetScreenSize(int width, int height);
    void SetPosition(const glm::vec2& pos);
    void SetZoom(float z);

    [[nodiscard]] const glm::vec2& GetPosition() const;
    [[nodiscard]] float GetZoom() const;

    [[nodiscard]] glm::mat4 GetProjectionMatrix() const;

    [[nodiscard]] bool IsInView(const glm::vec2& pos, float radius, glm::vec2 viweportSize) const
    {
        glm::vec2 camPos = GetPosition();
        glm::vec2 halfSize = viweportSize * 0.5f; 

        return !(pos.x + radius < camPos.x - halfSize.x ||
            pos.x - radius > camPos.x + halfSize.x ||
            pos.y + radius < camPos.y - halfSize.y ||
            pos.y - radius > camPos.y + halfSize.y);
    }

private:
    glm::vec2 position = glm::vec2(0.0f);
    float zoom = 1.0f;
    int screenWidth = 800;
    int screenHeight = 600;
};
