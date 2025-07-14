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

/**
 * @brief
 * @details
 * @author Jinwoo Choi
 * @date 2025-07-10
 */

class StateManager;

class RenderManager
{
    friend ObjectManager;
    friend StateManager;

public:
    void RegisterShader(const std::string& tag, const std::vector<std::pair<ShaderStage, std::string>>& sources);
    void RegisterTexture(const std::string& tag, const std::string& path, const TextureSettings& settings);
    void RegisterMesh(const std::string& tag, const std::vector<float>& vertices, const std::vector<unsigned int>& indices = {});
    void RegisterMaterial(const std::string& tag, const std::string& shaderTag, const std::unordered_map<std::string/*uniformName*/, std::string/*textureTag*/>& textureBindings);
   
    [[nodiscard]] Shader* GetShaderByTag(const std::string& tag) { return shaderMap[tag].get(); }
  
    [[nodiscard]] Texture* GetTextureByTag(const std::string& tag) { return textureMap[tag].get(); }
    
    [[nodiscard]] Mesh* GetMeshByTag(const std::string& tag) { return meshMap[tag].get(); }
  
    [[nodiscard]] Material* GetMaterialByTag(const std::string& tag) { return materialMap[tag].get(); }

protected:
    void BeginFrame();
    void Submit(std::function<void()>&& drawFunc, unsigned int layer = 0);
    void EndFrame();

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
