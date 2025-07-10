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
    bool IsAlive() { return isAlive; }

    /**
     * Marks this object as no longer alive.
     */
    void Kill() { isAlive = false; }

    /** Sets the string ID of this object. */
    void SetID(const std::string& id) { objectID = id; }

    /** Returns the string ID of this object. */
    const std::string& GetID() const { return objectID; }

private:
    bool isAlive = true;
    std::string objectID;
};

/**
 * @brief Manages the lifecycle and lookup of all GameObjects.
 * @details
 * ObjectManager holds all active and pending GameObjects in a state or scene.
 * It provides mechanisms to add, initialize, update, draw, and remove objects.
 *
 * - Active objects are stored in `objects`.
 * - Newly added objects are deferred in `pendingObjects` and initialized later.
 * - Fast lookup is supported via `objectMap`, which holds raw pointers.
 *   (Ownership remains with the internal vectors.)
 *
 * @author Jinwoo Choi
 * @date 2025-07-09
 */
class ObjectManager
{
public:
    /**
     * @brief Adds a new object to be managed.
     * @param obj Unique pointer to the new object.
     * @param ID Optional string ID. If set, used for lookup via FindByID().
     */
    void AddObject(std::unique_ptr<GameObject> obj, std::string ID = "");

    /** Initializes all currently registered objects. */
    void InitAll();

    /**
     * @brief Updates all active objects and processes pending additions and removals.
     * @param dt Delta time.
     * @param engineContext Engine-wide access to shared systems.
     */
    void UpdateAll(float dt, const EngineContext& engineContext);

    /** Moves all pending objects into the active list and calls their Init/LateInit. */
    void AddAllPendingObjects();

    /** Removes all dead objects (IsAlive() == false) after calling their Free/LateFree. */
    void EraseDeadObjects();

    /**
     * @brief Draws all alive objects.
     * @param engineContext Rendering and system context passed to each object.
     */
    void DrawAll(const EngineContext& engineContext);

    /** Frees and clears all objects. Called when state ends. */
    void FreeAll();

    /**
     * @brief Finds an object by its ID.
     * @param id String ID to search.
     * @return Pointer to the object, or nullptr if not found.
     */
    GameObject* FindByID(const std::string& id) const;

private:
    /** All currently active objects. */
    std::vector<std::unique_ptr<GameObject>> objects;

    /** Objects waiting to be initialized in the next frame. */
    std::vector<std::unique_ptr<GameObject>> pendingObjects;

    /**
     * Map of object IDs to raw pointers for fast lookup.
     * Ownership is held by `objects` or `pendingObjects`.
     */
    std::unordered_map<std::string, GameObject*> objectMap;
};
