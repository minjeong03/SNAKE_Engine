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
#include "Font.h"
#include "GameObject.h"
#include "InstanceBatchKey.h"
#include "RenderLayerManager.h"

struct TextInstance;
class SNAKE_Engine;
class StateManager;

using TextureTag = std::string;
using UniformName = std::string;
using FilePath = std::string;
using RenderCommand = std::function<void()>;

using ShaderMap = std::map<Shader*, std::map<InstanceBatchKey, std::vector<std::pair<Object*, Camera2D*>>>>;
using RenderMap = std::array<ShaderMap, RenderLayerManager::MAX_LAYERS>;

class RenderManager
{
    friend ObjectManager;
    friend StateManager;
    friend SNAKE_Engine;

public:
    void RegisterShader(const std::string& tag, const std::vector<std::pair<ShaderStage, FilePath>>& sources);

    void RegisterShader(const std::string& tag, std::unique_ptr<Shader> shader);

    void RegisterTexture(const std::string& tag, const FilePath& path, const TextureSettings& settings);

    void RegisterTexture(const std::string& tag, std::unique_ptr<Texture> texture);

    void RegisterMesh(const std::string& tag, const std::vector<float>& vertices, const std::vector<unsigned int>& indices = {});

    void RegisterMesh(const std::string& tag, std::unique_ptr<Mesh> mesh);

    void RegisterMaterial(const std::string& tag, const std::string& shaderTag, const std::unordered_map<UniformName, TextureTag>& textureBindings);

    void RegisterMaterial(const std::string& tag, std::unique_ptr<Material> material);

    void RegisterFont(const std::string& tag, const std::string& ttfPath, uint32_t pixelSize);

    void RegisterFont(const std::string& tag, std::unique_ptr<Font> font);

    void RegisterRenderLayer(const std::string& tag);

    [[nodiscard]] Shader* GetShaderByTag(const std::string& tag) { return shaderMap[tag].get(); }

    [[nodiscard]] Texture* GetTextureByTag(const std::string& tag) { return textureMap[tag].get(); }

    [[nodiscard]] Mesh* GetMeshByTag(const std::string& tag) { return meshMap[tag].get(); }

    [[nodiscard]] Material* GetMaterialByTag(const std::string& tag) { return materialMap[tag].get(); }

    [[nodiscard]] Font* GetFontByTag(const std::string& tag) { return fontMap[tag].get(); }

    void Submit(std::function<void()>&& drawFunc);

    void FlushDrawCommands(const EngineContext& engineContext);

    void SetViewport(int x, int y, int width, int height);

    void ClearBackground(int x, int y, int width, int height, glm::vec4 color);

    RenderLayerManager& GetRenderLayerManager();
private:
    void Init(const EngineContext& engineContext);

    void BuildRenderMap(const std::vector<Object*>& source, Camera2D* camera);

    void SubmitRenderMap(const EngineContext& engineContext);

    void Submit(const EngineContext& engineContext, const std::vector<Object*>& objects, Camera2D* camera);

    std::unordered_map<std::string, std::unique_ptr<Shader>> shaderMap;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textureMap;
    std::unordered_map<std::string, std::unique_ptr<Mesh>> meshMap;
    std::unordered_map<std::string, std::unique_ptr<Material>> materialMap;
    std::unordered_map<std::string, std::unique_ptr<Font>> fontMap;
    std::vector<RenderCommand> renderQueue;
    RenderMap renderMap;
    RenderLayerManager renderLayerManager;
};

class FrustumCuller
{
public:
    static void CullVisible(const Camera2D& camera, const std::vector<Object*>& allObjects,
        std::vector<Object*>& outVisibleList, glm::vec2 viewportSize);
};
