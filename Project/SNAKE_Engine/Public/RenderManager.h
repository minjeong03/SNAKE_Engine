#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct EngineContext;
/**
 * @brief
 * @details
 * @author Jinwoo Choi
 * @date 2025-07-10
 */

class Texture
{
};

class Shader
{
};

class Mesh
{
};

class Material
{
public:
    Material(Shader* _shader, Texture* _texture);
    [[nodiscard]]
    Shader* GetShader() const { return shader; }
    [[nodiscard]]
    Texture* GetTexture() const { return texture; }

private:
    Shader* shader;
    Texture* texture;
};


class RenderManager
{
public:
    void BeginFrame();
    void Submit(std::function<void()>&& drawFunc, unsigned int layer = 0);
    void EndFrame();
    void RegisterShader(const std::string& tag, std::unique_ptr<Shader> shader);
    void RegisterTexture(const std::string& tag, std::unique_ptr<Texture> texture);
    void RegisterMesh(const std::string& tag, std::unique_ptr<Mesh> mesh);
    [[nodiscard]]
    std::unique_ptr<Material> CreateMaterial(const std::string& shaderTag, const std::string& textureTag);
    void LoadTexture(const std::string& path, const std::string& textureTag);

private:
    struct RenderCommand {
        std::function<void()> drawFunc;
        unsigned int renderLayer;
    };

    std::unordered_map<std::string, std::unique_ptr<Shader>> shaderMap;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textureMap;
    std::unordered_map<std::string, std::unique_ptr<Mesh>> meshMap;
    std::vector<RenderCommand> renderQueue;
};