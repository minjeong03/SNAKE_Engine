#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "../ThirdParty/glm/glm.hpp"
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

class Material
{
public:
    Material(Shader* _shader, Texture* _texture);
    Shader* GetShader() const { return shader; }
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
    void RegisterShader(const std::string& tag, Shader* shader);
    void RegisterTexture(const std::string& tag, Texture* texture);
    std::unique_ptr<Material> CreateMaterial(const std::string& shaderTag, const std::string& textureTag);
    void LoadTexture(const std::string& path, const std::string& textureTag);
private:
    struct RenderCommand {
        std::function<void()> drawFunc;
        unsigned int renderLayer;
    };
    //TODO: change pointers to a unique pointer
    //      these can be managed in rendermanager and return raw pointer for usage
    std::unordered_map<std::string, Shader*> shaderMap;
    std::unordered_map<std::string, Texture*> textureMap;
    std::vector<RenderCommand> renderQueue;
};