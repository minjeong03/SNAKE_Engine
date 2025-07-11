#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Material.h"
#include "Shader.h"
#include "Texture.h"

/**
 * @brief
 * @details
 * @author Jinwoo Choi
 * @date 2025-07-10
 */

class Mesh
{
	
};

class RenderManager
{
public:
    void BeginFrame();
    void Submit(std::function<void()>&& drawFunc, unsigned int layer = 0);
    void EndFrame();
    void RegisterShader(const std::string& tag, std::unique_ptr<Shader> shader);
    void RegisterShader(const std::string& tag, const std::vector<std::pair<ShaderStage, std::string>>& sources);
    void LoadAndRegisterTexture(const std::string& tag, const std::string& path, const TextureSettings& settings);
    void RegisterMesh(const std::string& tag, std::unique_ptr<Mesh> mesh);
    [[nodiscard]]
    std::unique_ptr<Material> CreateMaterial(const std::string& shaderTag, const std::unordered_map<std::string, std::string>& textureBindings // uniformName ¡æ textureTag
    );

private:
    struct RenderCommand
	{
        std::function<void()> drawFunc;
        unsigned int renderLayer;
    };

    std::unordered_map<std::string, std::unique_ptr<Shader>> shaderMap;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textureMap;
    std::unordered_map<std::string, std::unique_ptr<Mesh>> meshMap;
    std::vector<RenderCommand> renderQueue;
};