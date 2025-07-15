#pragma once
#include "../ThirdParty/glm/glm.hpp"

/**
 * @brief A simple 2D orthographic camera for screen-space rendering.
 *
 * @details
 * Camera2D handles position, zoom, and projection matrix generation.
 * It also provides 2D frustum culling via bounding-circle intersection.
 */
class Camera2D
{
public:
    /**
     * @brief Constructs a 2D camera with a given screen size.
     *
     * @param screenWidth Width of the screen in pixels.
     * @param screenHeight Height of the screen in pixels.
     */
    Camera2D(int screenWidth = 800, int screenHeight = 600);

    /**
     * @brief Sets the screen size used for projection calculation.
     *
     * @param width Width in pixels.
     * @param height Height in pixels.
     */
    void SetScreenSize(int width, int height);

    /**
     * @brief Sets the world position of the camera.
     *
     * @param pos New center position of the camera in world space.
     */
    void SetPosition(const glm::vec2& pos);

    /**
     * @brief Sets the zoom level of the camera.
     *
     * @param z Zoom factor (1.0 = default scale).
     */
    void SetZoom(float z);

    /**
     * @brief Returns the current position of the camera.
     *
     * @return Position in world space.
     */
    [[nodiscard]] const glm::vec2& GetPosition() const;

    /**
     * @brief Returns the current zoom factor.
     *
     * @return Zoom value.
     */
    [[nodiscard]] float GetZoom() const;

    /**
     * @brief Computes and returns the orthographic projection matrix.
     *
     * @return 4x4 projection matrix based on screen size and zoom.
     */
    [[nodiscard]] glm::mat4 GetProjectionMatrix() const;

    /**
     * @brief Checks if a bounding circle is visible within the camera view.
     *
     * @details
     * Performs a simple AABB-circle intersection check using viewport bounds
     * and the camera's current position and zoom level.
     *
     * @param pos World-space center of the object.
     * @param radius Bounding radius of the object.
     * @param viewportSize Current viewport size in pixels.
     * @return True if the object is visible, false if culled.
     */
    [[nodiscard]] bool IsInView(const glm::vec2& pos, float radius, glm::vec2 viewportSize) const
    {
        glm::vec2 camPos = GetPosition();
        glm::vec2 halfSize = viewportSize * 0.5f;

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
