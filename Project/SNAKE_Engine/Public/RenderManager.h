#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

class StateManager;

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
    void RegisterShader(const std::string& tag, const std::vector<std::pair<ShaderStage, std::string>>& sources);

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
    void RegisterTexture(const std::string& tag, const std::string& path, const TextureSettings& settings);

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
    void RegisterMaterial(const std::string& tag, const std::string& shaderTag, const std::unordered_map<std::string, std::string>& textureBindings);

    /**
     * @brief Registers an existing material object by ownership transfer.
     *
     * @param tag ID to assign.
     * @param material Unique material instance.
     */
    void RegisterMaterial(const std::string& tag, std::unique_ptr<Material> material);

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

private:
    struct RenderCommand
    {
        std::function<void()> drawFunc;
        unsigned int renderLayer;
    };

    std::unordered_map<std::string, std::unique_ptr<Shader>> shaderMap;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textureMap;
    std::unordered_map<std::string, std::unique_ptr<Mesh>> meshMap;
    std::unordered_map<std::string, std::unique_ptr<Material>> materialMap;
    std::vector<RenderCommand> renderQueue;
};
