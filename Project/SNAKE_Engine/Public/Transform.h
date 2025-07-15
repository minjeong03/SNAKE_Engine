#pragma once
#include "../ThirdParty/glm/glm.hpp"

/**
 * @brief Stores 2D position, rotation, and scale for an object.
 *
 * Provides transformation data and automatically tracks changes for matrix recomputation.
 */
class Transform2D
{
public:
    /**
     * @brief Constructs a transform with default values: position (0,0), rotation 0, scale (1,1).
     */
    Transform2D()
        : position(0.f), rotation(0.f), scale(1.f),
        matrix(1.f), isChanged(true)
    {
    }

    /**
     * @brief Sets the object's world-space position.
     * @param pos New 2D position.
     */
    void SetPosition(const glm::vec2& pos)
    {
        position = pos;
        isChanged = true;
    }

    /**
     * @brief Offsets the current position by a delta value.
     * @param pos 2D offset vector.
     */
    void AddPosition(const glm::vec2& pos)
    {
        position += pos;
        isChanged = true;
    }

    /**
     * @brief Sets the object's rotation in radians.
     * @param rot Rotation angle in radians.
     */
    void SetRotation(float rot)
    {
        rotation = rot;
        isChanged = true;
    }

    /**
     * @brief Sets the scale for the object.
     * @param scl New 2D scale vector.
     */
    void SetScale(const glm::vec2& scl)
    {
        scale = scl;
        isChanged = true;
    }

    /**
     * @brief Gets the current position value.
     * @return Reference to the internal position vector.
     */
    const glm::vec2& GetPosition() const { return position; }

    /**
     * @brief Gets the current rotation value in radians.
     * @return Rotation value.
     */
    float GetRotation() const { return rotation; }

    /**
     * @brief Gets the current scale value.
     * @return Reference to the internal scale vector.
     */
    const glm::vec2& GetScale() const { return scale; }

    /**
     * @brief Returns the transformation matrix, updating it if any value changed.
     * @return Reference to the internal 4x4 matrix.
     */
    glm::mat4& GetMatrix();

private:
    glm::vec2 position;
    float rotation;
    glm::vec2 scale;

    glm::mat4 matrix;
    bool isChanged;
};
