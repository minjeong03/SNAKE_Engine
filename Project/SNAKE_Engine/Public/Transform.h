#pragma once
#include "../ThirdParty/glm/glm.hpp"

/**
 * @brief Represents a 2D spatial transform with position, rotation, and scale.
 *
 * Used to define an object's position in world space, orientation, and scale.
 * Generates a 4x4 matrix for rendering, recomputed lazily when any value changes.
 *
 * Example usage:
 * @code
 * transform.SetPosition({100.f, 200.f});
 * glm::mat4 model = transform.GetMatrix();
 * @endcode
 */
class Transform2D
{
public:
    /**
     * @brief Constructs a transform with default values.
     *
     * Initializes position to (0, 0), rotation to 0 radians, and scale to (1, 1).
     */
    Transform2D()
        : position(0.f), rotation(0.f), scale(1.f),
        matrix(1.f), isChanged(true)
    {
    }

    /**
     * @brief Sets the object's position in world space.
     *
     * Also marks the internal matrix as needing update.
     *
     * @param pos New 2D position.
     */
    void SetPosition(const glm::vec2& pos)
    {
        position = pos;
        isChanged = true;
    }

    /**
     * @brief Moves the object by a given offset.
     *
     * Adds a relative delta to the current position.
     *
     * @param pos 2D offset vector.
     */
    void AddPosition(const glm::vec2& pos)
    {
        position += pos;
        isChanged = true;
    }

    /**
     * @brief Sets the rotation angle of the object.
     *
     * @param rot Angle in radians. Positive is counter-clockwise.
     */
    void SetRotation(float rot)
    {
        rotation = rot;
        isChanged = true;
    }

    /**
     * @brief Sets the scaling factor along X and Y.
     *
     * @param scl 2D vector representing scale.
     */
    void SetScale(const glm::vec2& scl)
    {
        scale = scl;
        isChanged = true;
    }

    /**
     * @brief Gets the current position value.
     *
     * @return Const reference to the internal position vector.
     */
    const glm::vec2& GetPosition() const { return position; }

    /**
     * @brief Gets the current rotation angle.
     *
     * @return Rotation in radians.
     */
    float GetRotation() const { return rotation; }

    /**
     * @brief Gets the current scale vector.
     *
     * @return Const reference to the scale.
     */
    const glm::vec2& GetScale() const { return scale; }

    /**
     * @brief Returns the combined transformation matrix.
     *
     * Lazily updates and returns the transformation matrix representing
     * Translate * Rotate * Scale in 2D space.
     *
     * @return Reference to the 4x4 matrix.
     *
     * Example usage:
     * @code
     * glm::mat4 modelMatrix = transform.GetMatrix();
     * @endcode
     */
    glm::mat4& GetMatrix();

private:
    glm::vec2 position;
    float rotation;
    glm::vec2 scale;

    glm::mat4 matrix;
    bool isChanged;
};
