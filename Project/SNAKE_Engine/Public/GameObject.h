#pragma once
#include <string>
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"

class FrustumCuller;
struct EngineContext;

/**
 * @brief Base class for all in-game objects.
 * @details
 * GameObject provides a generic interface for any object that participates
 * in the game world, including enemies, bullets, UI elements, etc.
 *
 * The object lifecycle consists of:
 * - Init(): Primary initialization
 * - LateInit(): Secondary initialization after all objects are set up
 * - Update(): Per-frame logic
 * - Draw(): Per-frame rendering
 * - Free(): Cleanup before removal
 * - LateFree(): Final cleanup after dependencies are unlinked
 *
 * GameObjects are managed by ObjectManager and can be identified by string IDs.
 *
 * @author Jinwoo Choi
 * @date 2025-07-09
 */
class GameObject
{
    friend FrustumCuller;
public:
    /** Called once when the object is added (for initial setup). */
    virtual void Init(const EngineContext& engineContext) = 0;

    /** Called after Init(), once all objects are registered (cross-object setup). */
    virtual void LateInit(const EngineContext& engineContext) = 0;

    /** Called every frame for game logic. */
    virtual void Update(float dt, const EngineContext& engineContext) = 0;

    /** Called every frame for rendering. */
    virtual void Draw(const EngineContext& engineContext) = 0;

    /** Called when the object is about to be destroyed (unlink dependencies). */
    virtual void Free(const EngineContext& engineContext) = 0;

    /** Called after Free(), used to release resources. */
    virtual void LateFree(const EngineContext& engineContext) = 0;

    /** Virtual destructor. */
    virtual ~GameObject() = default;

    /** Returns whether this object is still alive. */
    [[nodiscard]] const bool& IsAlive() const { return isAlive; }

    [[nodiscard]] const bool& IsVisible() const { return isVisible; }

    void SetVisibility(bool _isVisible) { isVisible = _isVisible; }
    /**
     * Marks this object as no longer alive.
     */
    void Kill() { isAlive = false; }

    /** Sets the string ID of this object. */
    void SetTag(const std::string& tag) { objectTag = tag; }

    /** Returns the string ID of this object. */
    [[nodiscard]] const std::string& GetTag() const { return objectTag; }

    [[nodiscard]] const unsigned int& GetRenderLayer() const { return renderLayer; }

    void SetMaterial(Material* _material) { material = _material; }

    [[nodiscard]] Material* GetMaterial() const { return material; }

    void SetMesh(Mesh* _mesh) { mesh = _mesh; }

    [[nodiscard]] Mesh* GetMesh() const { return mesh; }

    [[nodiscard]] bool CanBeInstanced() const;

    [[nodiscard]] glm::mat4& GetTransformMatrix() { return transform2D.GetMatrix(); }

    [[nodiscard]] Transform2D& GetTransform2D() { return transform2D; }



protected: //to give direct access for inherited gameobject classes
    bool isAlive = true;
    bool isVisible = true;

    std::string objectTag;

    unsigned int renderLayer = 0;
    Material* material = nullptr;
    Mesh* mesh = nullptr;
    Transform2D transform2D;

private:
    [[nodiscard]] float GetBoundingRadius() const
    {
        glm::vec2 halfSize = mesh ? mesh->GetLocalBoundsHalfSize() : glm::vec2(0.5f);
        glm::vec2 scaled = halfSize * transform2D.GetScale();
        return glm::length(scaled);
    }
};

inline bool GameObject::CanBeInstanced() const
{
    if (!mesh || !material) return false;

    if (!material->IsInstancingSupported()) return false;

    return true;
}
