#pragma once
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera2D.h"
#include "GameObject.h"
#include "RenderLayerManager.h"

struct InstanceBatchKey;
class StateManager;

using TextureTag = std::string;
using UniformName = std::string;
using FilePath = std::string;
using RenderCommand = std::function<void()>;





/**
 * @brief Render batching structure organized by Shader and instancing key.
 *
 * @details
 * Each Shader* points to a map from (Mesh + Material) -> list of GameObjects.
 * Used internally by ObjectManager to prepare instanced draw submissions.
 */
using ShaderMap = std::map<Shader*, std::map<InstanceBatchKey, std::vector<GameObject*>>>;

/**
 * @brief Fixed-size array mapping render layers to ShaderMaps.
 *
 * @details
 * RenderMap is a 16-element array (MAX_LAYERS) where each entry represents a render layer.
 * Each layer holds a ShaderMap, which batches objects by Shader and instance data.
 *
 * Internally used by RenderManager for layered and instanced rendering.
 */
using RenderMap = std::array<ShaderMap, RenderLayerManager::MAX_LAYERS>;


/**
 * @brief Central renderer and asset registry for GPU resources.
 *
 * @details
 * RenderManager handles registration and lookup for all GPU-bound assets including
 * shaders, meshes, textures, and materials. It also manages per-frame draw submission,
 * and issuing render commands to the GPU.
 *
 * The rendering flow consists of:
 * - Submit(): collects draw calls
 * - FlushDrawCommands(): executes all sorted draw calls
 */
class RenderManager
{
    friend ObjectManager;
    friend StateManager;

public:
    /**
     * @brief Registers and compiles a new shader from file sources.
     *
     * @details
     * Each source is specified as a (ShaderStage, filepath) pair. The resulting
     * Shader is linked and stored by tag.
     * If the tag already exists, a warning is logged and no action is taken.
     *
     * @param tag Unique identifier for the shader.
     * @param sources List of stage-filepath pairs.
     *
     * @code
     * renderManager.RegisterShader("basic", {
     *   { ShaderStage::Vertex, "shaders/basic.vert" },
     *   { ShaderStage::Fragment, "shaders/basic.frag" }
     * });
     * @endcode
     */
    void RegisterShader(const std::string& tag, const std::vector<std::pair<ShaderStage, FilePath>>& sources);

    /**
     * @brief Registers a user-created shader into the manager.
     *
     * @param tag Identifier key for retrieval.
     * @param shader Shader instance to take ownership of.
     */
    void RegisterShader(const std::string& tag, std::unique_ptr<Shader> shader);

    /**
     * @brief Loads and registers a texture from file.
     *
     * @details
     * Loads the image, applies sampling settings, and stores the texture by tag.
     * If the tag is already in use, the texture is not replaced.
     *
     * @param tag Texture ID.
     * @param path Path to the image file.
     * @param settings Texture loading configuration.
     */
    void RegisterTexture(const std::string& tag, const FilePath& path, const TextureSettings& settings);

    /**
     * @brief Registers a pre-created texture object.
     *
     * @param tag ID to associate with this texture.
     * @param texture Unique pointer to a Texture instance.
     */
    void RegisterTexture(const std::string& tag, std::unique_ptr<Texture> texture);

    /**
     * @brief Registers a mesh from vertex and optional index data.
     *
     * @param tag Mesh name.
     * @param vertices Vertex buffer.
     * @param indices Index buffer (optional).
     */
    void RegisterMesh(const std::string& tag, const std::vector<float>& vertices, const std::vector<unsigned int>& indices = {});

    /**
     * @brief Registers an existing mesh object by ownership transfer.
     *
     * @param tag ID to assign.
     * @param mesh Unique mesh instance.
     */
    void RegisterMesh(const std::string& tag, std::unique_ptr<Mesh> mesh);

    /**
     * @brief Creates a material using a registered shader and texture bindings.
     *
     * @details
     * If a texture in the bindings does not exist, a warning is logged and skipped.
     * The material is not created if the shader is missing.
     *
     * @param tag Material ID.
     * @param shaderTag Shader to use.
     * @param textureBindings Map of uniform names to texture tags.
     */
    void RegisterMaterial(const std::string& tag, const std::string& shaderTag, const std::unordered_map<UniformName, TextureTag>& textureBindings);

    /**
     * @brief Registers an existing material object by ownership transfer.
     *
     * @param tag ID to assign.
     * @param material Unique material instance.
     */
    void RegisterMaterial(const std::string& tag, std::unique_ptr<Material> material);

    /**
     * @brief Registers a new custom render layer by name.
     *
     * @details
     * Adds a new named layer to the internal RenderLayerManager. This allows
     * GameObjects to specify which layer they belong to for rendering order.
     * Layers are drawn in increasing order (layer 0 drawn first).
     *
     * @param tag String identifier for the new render layer (max 16 layers).
     *
     * @code
     * renderManager.RegisterRenderLayer("UI"); // layer 0
     * renderManager.RegisterRenderLayer("Enemies"); // layer 1
     * @endcode
     */
    void RegisterRenderLayer(const std::string& tag);

    /**
     * @brief Retrieves a shader by tag.
     *
     * @param tag Identifier name.
     * @return Pointer to Shader.
     */
    [[nodiscard]] Shader* GetShaderByTag(const std::string& tag) { return shaderMap[tag].get(); }

    /**
     * @brief Retrieves a texture by tag.
     *
     * @param tag Texture identifier.
     * @return Pointer to Texture.
     */
    [[nodiscard]] Texture* GetTextureByTag(const std::string& tag) { return textureMap[tag].get(); }

    /**
     * @brief Retrieves a mesh by tag.
     *
     * @param tag Mesh identifier.
     * @return Pointer to Mesh.
     */
    [[nodiscard]] Mesh* GetMeshByTag(const std::string& tag) { return meshMap[tag].get(); }

    /**
     * @brief Retrieves a material by tag.
     *
     * @param tag Material ID.
     * @return Pointer to Material.
     */
    [[nodiscard]] Material* GetMaterialByTag(const std::string& tag) { return materialMap[tag].get(); }

    /**
     * @brief Queues a render command for execution this frame.
     *
     * @details
     * Commands are stored with a layer number and executed in descending order (higher layers are drawn in front).
     *
     * @param drawFunc The rendering function (e.g., bind, draw).
     */
    void Submit(std::function<void()>&& drawFunc);

    /**
     * @brief Prepares the render queue for the frame.
     *
     * @details
     * Clears all previously submitted draw calls.
     */
    void ClearDrawCommands();

    /**
     * @brief Sorts and executes all submitted draw functions (higher layers are drawn in front).
     */
    void FlushDrawCommands();

    /**
     * @brief Schedules a viewport change.
     *
     * @details
     * Uses OpenGL's glViewport within a render command.
     *
     * @param x X origin.
     * @param y Y origin.
     * @param width Width of viewport.
     * @param height Height of viewport.
     */
    void SetViewport(int x, int y, int width, int height);

    /**
     * @brief Schedules a background clear for a region.
     *
     * @details
     * Enables scissor test, clears to given color, then disables scissor.
     * (used scissor test for clearing specified area)
     *
     * @param x Origin X.
     * @param y Origin Y.
     * @param width Region width.
     * @param height Region height.
     * @param color Clear color.
     */
    void ClearBackground(int x, int y, int width, int height, glm::vec4 color);

    /**
     * @brief Provides access to the internal render layer manager.
     *
     * @details
     * Returns a reference to the RenderLayerManager, which can be used to
     * query existing layer indices or manually assign layers to GameObjects.
     *
     * @return Reference to the RenderLayerManager instance.
     *
     * @code
     * std::string layerName = renderManager.GetLayerManager().GetLayerName(0);
     * @endcode
     */
    RenderLayerManager& GetRenderLayerManager();

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

    /**
     * @brief Internal helper for visibility culling and batched submission.
     *
     * @details
     * This method performs frustum culling on the given objects using the provided camera,
     * builds a render map, and delegates to SubmitRenderMap().
     * Intended for internal use during per-frame rendering.
     *
     * Called by: ObjectManager::DrawAll, DrawObjects, DrawObjectsByTag
     * Not intended for direct external use.
     *
     * @param engineContext Engine services used during submission.
     * @param allObjects List of potentially visible GameObjects.
     * @param camera Active camera for culling and projection.
     */
    void Submit(const EngineContext& engineContext, const std::vector<GameObject*>& allObjects, Camera2D* camera);

    std::unordered_map<std::string, std::unique_ptr<Shader>> shaderMap;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textureMap;
    std::unordered_map<std::string, std::unique_ptr<Mesh>> meshMap;
    std::unordered_map<std::string, std::unique_ptr<Material>> materialMap;
    std::vector<RenderCommand> renderQueue;

    RenderLayerManager renderLayerManager;
};


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

            if (camera.IsInView(pos, radius, viewportSize / camera.GetZoom()))
                outVisibleList.push_back(obj);
        }
    }
};
