#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Camera2D.h"
#include "GameObject.h"

struct EngineContext;
class Camera2D;




/**
 * @brief Manages the lifecycle, visibility, and rendering of GameObjects in a scene.
 *
 * @details
 * ObjectManager stores and maintains all GameObjects that exist in a scene or state.
 * It handles registration, initialization, per-frame updates, visibility culling, sorting,
 * instanced and non-instanced rendering, and safe destruction of dead objects.
 *
 * Object access:
 * - Active objects are held in 'objects'.
 * - Pending objects are added to 'pendingObjects' and initialized next frame.
 * - 'objectMap' maps string tags to raw pointers for ID-based lookup.
 */
class ObjectManager
{
public:
    /**
     * @brief Adds a new GameObject to the manager.
     *
     * @details
     * If a non-empty tag is provided and already exists, a warning is logged.
     * The object is not immediately initialized-see AddAllPendingObjects().
     *
     * @param obj Unique pointer to the GameObject.
     * @param tag Optional string ID to allow FindByID() access.
     *
     * @code
     * objectManager.AddObject(std::make_unique<Enemy>(), "enemy_01");
     * @endcode
     */
    void AddObject(std::unique_ptr<GameObject> obj, const std::string& tag = "");

    /**
     * @brief Calls Init() and LateInit() on all active objects.
     *
     * Should be called once when the scene is fully loaded.
     *
     * @param engineContext Shared engine data (RenderManager, InputManager, etc.)
     */
    void InitAll(const EngineContext& engineContext);

    /**
     * @brief Updates all alive objects and processes creation/deletion.
     *
     * @details
     * - Dead objects are removed after Free() and LateFree().
     * - New pending objects are initialized and promoted to active.
     *
     * @param dt Delta time.
     * @param engineContext Engine context.
     */
    void UpdateAll(float dt, const EngineContext& engineContext);

    /**
     * @brief Initializes all pending objects and promotes them to active list.
     *
     * @param engineContext Context for object setup.
     */
    void AddAllPendingObjects(const EngineContext& engineContext);

    /**
     * @brief Deletes all objects marked as dead (IsAlive == false).
     *
     * @details
     * Dead objects are removed after calling Free() and LateFree().
     * They are also removed from internal maps and pointer lists.
     *
     * @param engineContext Context for resource cleanup.
     */
    void EraseDeadObjects(const EngineContext& engineContext);

    /**
     * @brief Draws all visible objects using view frustum culling and instancing.
     *
     * @details
     * - Culls objects outside of camera view.
     * - Builds and submits optimized render batches.
     *
     * @param engineContext Engine systems (includes RenderManager).
     * @param camera Camera used to determine visibility and projection.
     */
    void DrawAll(const EngineContext& engineContext, Camera2D* camera);

    /**
     * @brief Draws only the objects in the provided list using frustum culling.
     *
     * @param engineContext Shared systems.
     * @param camera Camera used for culling.
     * @param gameObjects List of GameObjects to attempt to draw.
     */
    void DrawObjects(const EngineContext& engineContext, Camera2D* camera, const std::vector<GameObject*>& gameObjects);

    /**
     * @brief Draws only the objects with the given tag if alive and visible.
     *
     * @param engineContext Context for draw submission.
     * @param camera Culling and projection camera.
     * @param tag Object ID to match.
     */
    void DrawObjectsWithTag(const EngineContext& engineContext, Camera2D* camera, const std::string& tag);

    /**
     * @brief Frees and clears all currently held objects.
     *
     * @details
     * Should be called when the GameState is shutting down.
     *
     * @param engineContext Context used during Free().
     */
    void FreeAll(const EngineContext& engineContext);

    /**
     * @brief Returns a pointer to the object with the given string ID.
     *
     * @param id String tag of object.
     * @return GameObject pointer, or nullptr if not found.
     */
    [[nodiscard]] GameObject* FindByID(const std::string& id) const;

private:

    std::vector<std::unique_ptr<GameObject>> objects;
    std::vector<std::unique_ptr<GameObject>> pendingObjects;
    std::vector<GameObject*> rawPtrObjects;

    std::unordered_map<std::string, GameObject*> objectMap;
};

