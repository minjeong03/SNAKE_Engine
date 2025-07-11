#include "RenderManager.h"
#include <algorithm>
#include <iostream>
#include "Debug.h"


void RenderManager::BeginFrame()
{
    renderQueue.clear();
}

void RenderManager::Submit(std::function<void()>&& drawFunc, unsigned int layer)
{
    renderQueue.push_back({ std::move(drawFunc), layer });
}

void RenderManager::EndFrame()
{
    std::sort(renderQueue.begin(), renderQueue.end(), 
        [](const RenderCommand& a, const RenderCommand& b) 
        {
			return a.renderLayer > b.renderLayer;
        });

    for (const auto& cmd : renderQueue) 
    {
        cmd.drawFunc();
    }
}

void RenderManager::RegisterShader(const std::string& tag, std::unique_ptr<Shader> shader)
{
    if (shaderMap.find(tag) == shaderMap.end())
        shaderMap[tag] = std::move(shader);
    else
        SNAKE_LOG("Shader with tag \"" << tag << "\" already registered.");
}

/*
 * Usage: 
 * renderManager.RegisterShader("basic", {
 * { ShaderStage::Vertex, "shaders/basic.vert" },
 * { ShaderStage::Fragment, "shaders/basic.frag" }
 * });
 */ 
void RenderManager::RegisterShader(const std::string& tag, const std::vector<std::pair<ShaderStage, std::string>>& sources)
{
    auto shader = std::make_unique<Shader>();

    for (const auto& [stage, path] : sources)
        shader->AttachFromFile(stage, path);

    shader->Link();
    shaderMap[tag] = std::move(shader);
}

void RenderManager::LoadAndRegisterTexture(const std::string& tag, const std::string& path, const TextureSettings& settings)
{
    if (textureMap.find(tag) == textureMap.end())
        textureMap[tag] = std::make_unique<Texture>(path, settings);
    else
        SNAKE_LOG("Texture with tag \"" << tag << "\" already registered.");
}

void RenderManager::RegisterMesh(const std::string& tag, std::unique_ptr<Mesh> mesh)
{
    if (meshMap.find(tag) == meshMap.end())
        meshMap[tag] = std::move(mesh);
    else  
        SNAKE_LOG("Mesh with tag \"" << tag << "\" already registered.");
}

std::unique_ptr<Material> RenderManager::CreateMaterial(const std::string& shaderTag, const std::unordered_map<std::string, std::string>& textureBindings)
{
    Shader* shader = shaderMap[shaderTag].get();
    auto material = std::make_unique<Material>(shader);

    for (const auto& [uniformName, textureTag] : textureBindings)
    {
        auto it = textureMap.find(textureTag);
        if (it != textureMap.end())
            material->SetTexture(uniformName, it->second.get());
        else
            SNAKE_ERR("Texture not found: " << textureTag);
    }

    return material;
}
