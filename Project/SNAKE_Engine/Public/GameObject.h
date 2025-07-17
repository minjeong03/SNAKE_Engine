#pragma once
#include "Mesh.h"
#include "Transform.h"


struct EngineContext;
class FrustumCuller;

/**
 * @brief Base interface for all game world objects.
 *
 * @details
 * GameObject is the root abstract class for all entities that exist in the game world,
 * such as players, enemies, bullets, or UI elements.
 *
 * Each object follows a standard lifecycle consisting of:
 * - Init(): Initialize this object
 * - LateInit(): Cross-object setup after all objects are registered
 * - Update(): Per-frame game logic
 * - Draw(): Per-frame rendering
 * - Free(): Cleanup before removal
 * - LateFree(): Final cleanup after object dependencies are unlinked
 *
 * GameObjects are managed by ObjectManager and tagged via string identifiers.
 */
class GameObject
{
    friend FrustumCuller;
public:
    /**
     * @brief Initializes the object during registration.
     *
     * @param engineContext Global engine services and context.
     */
    virtual void Init(const EngineContext& engineContext) = 0;

    /**
     * @brief Called after Init(), once all GameObjects are initialized.
     *
     * @param engineContext Global engine context.
     */
    virtual void LateInit(const EngineContext& engineContext) = 0;

    /**
     * @brief Per-frame logic update.
     *
     * @param dt Time delta since last frame.
     * @param engineContext Engine context for services and references.
     */
    virtual void Update(float dt, const EngineContext& engineContext) = 0;

    /**
     * @brief Per-frame rendering function.
     *
     * @param engineContext Engine-wide rendering and asset context.
     */
    virtual void Draw(const EngineContext& engineContext) = 0;

    /**
     * @brief Called before this object is destroyed or recycled.
     *
     * @param engineContext Engine-wide reference access.
     */
    virtual void Free(const EngineContext& engineContext) = 0;

    /**
     * @brief Final cleanup after Free().
     *
     * @param engineContext Used for any residual cleanup logic.
     */
    virtual void LateFree(const EngineContext& engineContext) = 0;

    /**
     * @brief Virtual destructor for safe polymorphic deletion.
     */
    virtual ~GameObject() = default;

    /**
     * @brief Checks if this object is still active and should be updated.
     *
     * @return True if object is alive.
     */
    [[nodiscard]] const bool& IsAlive() const;

    /**
     * @brief Checks if this object should be drawn this frame.
     *
     * @return True if visible.
     */
    [[nodiscard]] const bool& IsVisible() const;

    /**
     * @brief Sets the visibility state of this object.
     *
     * @param _isVisible Whether this object should be visible.
     */
    void SetVisibility(bool _isVisible);

    /**
     * @brief Marks this object for destruction.
     */
    void Kill();

    /**
     * @brief Sets the object's string identifier tag.
     *
     * @param tag Unique string used to tag this object.
     */
    void SetTag(const std::string& tag);

    /**
     * @brief Gets the object's string tag.
     *
     * @return Reference to string tag.
     */
    [[nodiscard]] const std::string& GetTag() const;

    /**
     * @brief Returns the render layer used for sorting.
     *
     * @return Render layer as uint8_t(unsigned char).
     */
    [[nodiscard]] const uint8_t& GetRenderLayer() const;

    /**
 * @brief Sets the render layer of the GameObject using a named tag.
 *
 * @details
 * Retrieves the numeric layer ID associated with the provided tag from the engine's RenderLayerManager.
 * If the tag is not registered, it defaults to layer 0.
 *
 * This allows objects to be grouped into rendering layers (e.g., background, UI).
 * Higher layers are rendered in front of lower layers.
 *
 * Example usage:
 * @code
 * player->SetRenderLayer(engineContext, "UI");
 * @endcode
 *
 * @param engineContext Reference to the engine-wide context.
 * @param tag Name of the render layer previously registered (e.g., "Background", "UI").
 */

    void SetRenderLayer(const EngineContext& engineContext, const std::string& tag);

    /**
     * @brief Sets the Material used by this object when rendered.
     *
     * @param _material Pointer to Material instance.
     */
    void SetMaterial(const EngineContext& engineContext, const std::string& tag);

    /**
     * @brief Gets the current Material used by this object.
     *
     * @return Pointer to Material.
     */
    [[nodiscard]] Material* GetMaterial() const;

    /**
     * @brief Sets the Mesh for this object.
     *
     * @param _mesh Pointer to Mesh.
     */
    void SetMesh(const EngineContext& engineContext, const std::string& tag);

    /**
     * @brief Gets the Mesh currently assigned to this object.
     *
     * @return Pointer to Mesh.
     */
    [[nodiscard]] Mesh* GetMesh() const;

    /**
     * @brief Determines if the GameObject can use GPU instancing.
     *
     * @details
     * Returns false if the object lacks a mesh or material,
     * or if the material does not support instancing.
     *
     * @return True if instancing is supported.
     */
    [[nodiscard]] bool CanBeInstanced() const;

    /**
     * @brief Gets the 2D transform matrix of this object.
     *
     * @return Reference to transformation matrix.
     */
    [[nodiscard]] glm::mat4& GetTransform2DMatrix();

    /**
     * @brief Accesses the 2D transform struct.
     *
     * @return Reference to internal Transform2D.
     */
    [[nodiscard]] Transform2D& GetTransform2D();

protected:
    bool isAlive = true;
    bool isVisible = true;

    std::string objectTag;

    Transform2D transform2D;

private:
    uint8_t renderLayer = 0;
    Material* material = nullptr;
    Mesh* mesh = nullptr;
    /**
     * @brief Calculates the bounding radius based on mesh and transform scale.
     *
     * @details
     * This value will be used for culling.
     * Culling: Objects that are outside the camera view will be ignored during the draw stage.
     * @return Scaled bounding radius.
     */
    [[nodiscard]] float GetBoundingRadius() const;

};

