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

void RenderManager::RegisterShader(const std::string& tag, std::unique_ptr<Shader> shader)
{
    if (shaderMap.find(tag) == shaderMap.end())
        shaderMap[tag] = std::move(shader);
    else
        std::cout << "Shader with tag \"" << tag << "\" already registered.\n";
}

void RenderManager::RegisterTexture(const std::string& tag, std::unique_ptr<Texture> texture)
{
    if (textureMap.find(tag) == textureMap.end())
		textureMap[tag] = std::move(texture);
    else
        std::cout << "Texture with tag \"" << tag << "\" already registered.\n";
}

void RenderManager::RegisterMesh(const std::string& tag, std::unique_ptr<Mesh> mesh)
{
    if (meshMap.find(tag) == meshMap.end())
        meshMap[tag] = std::move(mesh);
    else
        std::cout << "Mesh with tag \"" << tag << "\" already registered.\n";
}

std::unique_ptr<Material> RenderManager::CreateMaterial(const std::string& shaderTag, const std::string& textureTag)
{
    return std::make_unique<Material>(shaderMap[shaderTag].get(), textureMap[textureTag].get());
}

void RenderManager::LoadTexture(const std::string& path, const std::string& textureTag)
{
}
