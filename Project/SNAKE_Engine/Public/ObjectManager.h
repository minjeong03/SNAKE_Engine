#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <map>

#include "Camera2D.h"
#include "GameObject.h"

struct InstanceBatchKey;
class Camera2D;

class FrustumCuller
{
public:
    static void CullVisible(const Camera2D& camera, const std::vector<GameObject*>& allObjects,
        std::vector<GameObject*>& outVisibleList, glm::vec2 viewportSize)
    {
        outVisibleList.clear();
        for (GameObject* obj : allObjects)
        {
            if (!obj->IsAlive())
                continue;

            const glm::vec2& pos = obj->GetTransform2D().GetPosition();
            float radius = obj->GetBoundingRadius();

            if (camera.IsInView(pos, radius, viewportSize/camera.GetZoom()))
                outVisibleList.push_back(obj);
        }
    }
};

using ShaderMap = std::map<Shader*, std::map<InstanceBatchKey, std::vector<GameObject*>>>;
using RenderMap = std::map<int, ShaderMap>;

struct EngineContext;

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
    void AddObject(std::unique_ptr<GameObject> obj, const std::string& tag = "");

    /** Initializes all currently registered objects. */
    void InitAll(const EngineContext& engineContext);

    /**
     * @brief Updates all active objects and processes pending additions and removals.
     * @param dt Delta time.
     * @param engineContext Engine-wide access to shared systems.
     */
    void UpdateAll(float dt, const EngineContext& engineContext);

    /** Moves all pending objects into the active list and calls their Init/LateInit. */
    void AddAllPendingObjects(const EngineContext& engineContext);

    /** Removes all dead objects (IsAlive() == false) after calling their Free/LateFree. */
    void EraseDeadObjects(const EngineContext& engineContext);

    /**
     * @brief Draws all alive objects.
     * @param engineContext Rendering and system context passed to each object.
     */
    void DrawAll(const EngineContext& engineContext, Camera2D* camera);

    void DrawObjects(const EngineContext& engineContext, Camera2D* camera, const std::vector<GameObject*>& gameObjects);
    void DrawObjectsWithTag(const EngineContext& engineContext, Camera2D* camera, const std::string& tag);

    /** Frees and clears all objects. Called when state ends. */
    void FreeAll(const EngineContext& engineContext);

    /**
     * @brief Finds an object by its ID.
     * @param id String ID to search.
     * @return Pointer to the object, or nullptr if not found.
     */
    [[nodiscard]] GameObject* FindByID(const std::string& id) const;

private:

    void SubmitRenderMap(const EngineContext& engineContext, Camera2D* camera, const RenderMap& renderMap);
    [[nodiscard]] RenderMap BuildRenderMap(const std::vector<GameObject*>& source);
    /** All currently active objects. */
    std::vector<std::unique_ptr<GameObject>> objects;

    /** Objects waiting to be initialized in the next frame. */
    std::vector<std::unique_ptr<GameObject>> pendingObjects;

    std::vector<GameObject*> allRawPtrs;
    /**
     * Map of object IDs to raw pointers for fast lookup.
     * Ownership is held by `objects` or `pendingObjects`.
     */
    std::unordered_map<std::string, GameObject*> objectMap;
};
