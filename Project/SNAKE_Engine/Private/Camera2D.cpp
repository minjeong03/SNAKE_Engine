#include "Camera2D.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

Camera2D::Camera2D(int screenWidth, int screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight)
{
}

void Camera2D::SetScreenSize(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
}

void Camera2D::SetPosition(const glm::vec2& pos)
{
    position = pos;
}

void Camera2D::AddPosition(const glm::vec2& pos)
{
    position += pos;
}

void Camera2D::SetZoom(float z)
{
    zoom = z;
}

const glm::vec2& Camera2D::GetPosition() const
{
    return position;
}

float Camera2D::GetZoom() const
{
    return zoom;
}

glm::mat4 Camera2D::GetProjectionMatrix() const
{
    float halfW = static_cast<float>(screenWidth) * 0.5f / zoom;
    float halfH = static_cast<float>(screenHeight) * 0.5f / zoom;

    return glm::ortho(
        -halfW + position.x, halfW + position.x,
        -halfH + position.y, halfH + position.y,
        -1.0f, 1.0f
    );
}

bool Camera2D::IsInView(const glm::vec2& pos, float radius, glm::vec2 viewportSize) const
{
    glm::vec2 camPos = GetPosition();
    glm::vec2 halfSize = viewportSize * 0.5f;

    return !(pos.x + radius < camPos.x - halfSize.x ||
        pos.x - radius > camPos.x + halfSize.x ||
        pos.y + radius < camPos.y - halfSize.y ||
        pos.y - radius > camPos.y + halfSize.y);
}
