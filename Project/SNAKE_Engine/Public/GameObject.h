#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

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
public:
    /** Called once when the object is added (for initial setup). */
    virtual void Init() = 0;

    /** Called after Init(), once all objects are registered (cross-object setup). */
    virtual void LateInit() = 0;

    /** Called every frame for game logic. */
    virtual void Update(float dt, const EngineContext& engineContext) = 0;

    /** Called every frame for rendering. */
    virtual void Draw(const EngineContext& engineContext) = 0;

    /** Called when the object is about to be destroyed (unlink dependencies). */
    virtual void Free() = 0;

    /** Called after Free(), used to release resources. */
    virtual void LateFree() = 0;

    /** Virtual destructor. */
    virtual ~GameObject() = default;

    /** Returns whether this object is still alive. */
    [[nodiscard]]
    const bool& IsAlive() const { return isAlive; }

    [[nodiscard]]
	const bool& IsVisible() const { return isVisible; }

    void SetVisibility(bool _isVisible) { isVisible = _isVisible; }
    /**
     * Marks this object as no longer alive.
     */
    void Kill() { isAlive = false; }

    /** Sets the string ID of this object. */
    void SetID(const std::string& id) { objectID = id; }

    /** Returns the string ID of this object. */
    [[nodiscard]]
    const std::string& GetID() const { return objectID; }

    [[nodiscard]]
    const unsigned int& GetRenderLayer() const { return renderLayer; }
private:
    bool isAlive = true;
    bool isVisible = true;
    std::string objectID;
    unsigned int renderLayer = 0;
};
