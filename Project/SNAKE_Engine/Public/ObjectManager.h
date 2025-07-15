#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <map>

#include "Camera2D.h"
#include "GameObject.h"

struct InstanceBatchKey;
struct EngineContext;
class Camera2D;

/**
 * @brief Utility class for view frustum culling based on 2D bounding circles.
 *
 * @details
 * FrustumCuller performs visibility checks for a list of GameObjects using
 * simple 2D bounding radius tests against the camera's visible area.
 * This helps improve performance by skipping rendering of off-screen objects.
 */
class FrustumCuller
{
public:
    /**
   * @brief Filters out invisible GameObjects from a list.
   *
   * @details
   * For each object in the input list, this method:
   * - Skips objects that are not alive or not marked as visible.
   * - Checks if the object's 2D bounding circle intersects the camera view.
   * - Adds visible objects to the output list.
   *
   * @param camera Reference camera used for visibility testing.
   * @param allObjects Full list of objects to evaluate.
   * @param outVisibleList Output list of visible objects.
   * @param viewportSize Size of the rendering viewport in pixels.
   *
   * @code
   * std::vector<GameObject*> visible;
   * FrustumCuller::CullVisible(mainCamera, allObjects, visible, glm::vec2(1280, 720));
   * @endcode
   */
    static void CullVisible(const Camera2D& camera, const std::vector<GameObject*>& allObjects,
        std::vector<GameObject*>& outVisibleList, glm::vec2 viewportSize)
    {
        outVisibleList.clear();
        for (GameObject* obj : allObjects)
        {
            if (!obj->IsAlive() && !obj->IsVisible())
                continue;

            const glm::vec2& pos = obj->GetTransform2D().GetPosition();
            float radius = obj->GetBoundingRadius();

            if (camera.IsInView(pos, radius, viewportSize/camera.GetZoom()))
                outVisibleList.push_back(obj);
        }
    }
};

/**
 * @brief Render batching structure organized by Shader and instancing key.
 *
 * @details
 * Each Shader* points to a map from (Mesh + Material) -> list of GameObjects.
 * Used internally by ObjectManager to prepare instanced draw submissions.
 */
using ShaderMap = std::map<Shader*, std::map<InstanceBatchKey, std::vector<GameObject*>>>;

/**
 * @brief Top-level render command grouping by render layer.
 *
 * @details
 * RenderMap groups ShaderMap entries under a render layer key.
 * (higher layers are drawn in front)
 */
using RenderMap = std::map<int, ShaderMap>;

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

    /**
     * @brief Organizes GameObjects into a batched render map based on layer, shader, and instancing key.
     *
     * @details
     * Groups all visible GameObjects into a hierarchical map structure:
     * - First by render layer (int)
     * - Then by Shader*
     * - Then by (Mesh + Material) key
     *
     * Used for efficient render submission and instancing.
     *
     * @param source List of visible GameObjects to include in the batch.
     * @return RenderMap structure used for sorting and submission.
     */
    [[nodiscard]] RenderMap BuildRenderMap(const std::vector<GameObject*>& source);

    /**
     * @brief Submits all render commands from the given render map.
     *
     * @details
     * Iterates over the render map sorted by layer, shader, and instancing batch key.
     * - If the batch is instancing-compatible, submits a single instanced draw call.
     * - Otherwise, submits individual draw calls for each object.
     * Uniform binding and projection setup are batched and minimized.
     *
     * @param engineContext Context used to access render manager and global data.
     * @param camera Camera providing the projection matrix.
     * @param renderMap Pre-batched render data (see BuildRenderMap()).
     */
    void SubmitRenderMap(const EngineContext& engineContext, Camera2D* camera, const RenderMap& renderMap);


    std::vector<std::unique_ptr<GameObject>> objects;
    std::vector<std::unique_ptr<GameObject>> pendingObjects;
    std::vector<GameObject*> allRawPtrs;

    std::unordered_map<std::string, GameObject*> objectMap;
};

