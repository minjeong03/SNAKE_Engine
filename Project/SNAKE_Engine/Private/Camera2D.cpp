#include "Camera2D.h"
#include "glm/gtc/matrix_transform.hpp"

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
