#pragma once
#include "glm.hpp"

class Camera2D
{
public:
    Camera2D(int screenWidth = 800, int screenHeight = 600);

    void SetScreenSize(int width, int height);

    void SetPosition(const glm::vec2& pos);

    void AddPosition(const glm::vec2& pos);

    void SetZoom(float z);

    [[nodiscard]] const glm::vec2& GetPosition() const;

    [[nodiscard]] float GetZoom() const;

    [[nodiscard]] glm::mat4 GetProjectionMatrix() const;

    [[nodiscard]] bool IsInView(const glm::vec2& pos, float radius, glm::vec2 viewportSize) const;

    [[nodiscard]] int GetScreenWidth() const { return screenWidth; }

    [[nodiscard]] int GetScreenHeight() const { return screenHeight; }
private:
    glm::vec2 position = glm::vec2(0.0f);
    float zoom = 1.0f;
    int screenWidth = 800;
    int screenHeight = 600;
};
