#include "RenderManager.h"
#include <algorithm>
#include <iostream>

Material::Material(Shader* _shader, Texture* _texture):shader(_shader),texture(_texture)
{
}

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

void RenderManager::RegisterShader(const std::string& tag, Shader* shader)
{
    if (shaderMap.find(tag) == shaderMap.end())
        shaderMap[tag] = shader;
    else
        std::cout << "Shader with tag \"" << tag << "\" already registered.\n";
}

void RenderManager::RegisterTexture(const std::string& tag, Texture* texture)
{
    if (textureMap.find(tag) == textureMap.end())
		textureMap[tag] = texture;
    else
        std::cout << "Texture with tag \"" << tag << "\" already registered.\n";
}

std::unique_ptr<Material> RenderManager::CreateMaterial(const std::string& shaderTag, const std::string& textureTag)
{
    return std::make_unique<Material>(shaderMap[shaderTag], textureMap[textureTag]);
}

void RenderManager::LoadTexture(const std::string& path, const std::string& textureTag)
{
}
