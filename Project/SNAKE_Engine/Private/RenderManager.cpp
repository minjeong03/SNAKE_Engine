#include "RenderManager.h"
#include <algorithm>
#include <iostream>
#include "../ThirdParty/glad/gl.h"

#include "Debug.h"


void RenderManager::BeginFrame()
{
    renderQueue.clear();
}

void RenderManager::Submit(std::function<void()>&& drawFunc, unsigned int layer)
{
    renderQueue.push_back({ std::move(drawFunc), layer });
}

void RenderManager::FlushCommands()
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
    renderQueue.clear();
}

void RenderManager::SetViewport(int x, int y, int width, int height, int layer)
{
    Submit([=]() {
        glViewport(x, y, width, height);
        }, layer);
}

void RenderManager::ClearBackground(int x, int y, int width, int height, glm::vec4 color, int layer)
{
    Submit([=]() {
        glEnable(GL_SCISSOR_TEST);
        glScissor(x, y, width, height);
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);
        }, layer);
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
    if (shaderMap.find(tag) != shaderMap.end())
    {
        SNAKE_WRN("Mesh with tag \"" << tag << "\" already registered.");
        return;
    }
    auto shader = std::make_unique<Shader>();

    for (const auto& [stage, path] : sources)
        shader->AttachFromFile(stage, path);

    shader->Link();
    shaderMap[tag] = std::move(shader);
}

void RenderManager::RegisterTexture(const std::string& tag, const std::string& path, const TextureSettings& settings)
{
    if (textureMap.find(tag) != textureMap.end())
    {
        SNAKE_WRN("Texture with tag \"" << tag << "\" already registered.");
        return;
    }
	textureMap[tag] = std::make_unique<Texture>(path, settings);
}

void RenderManager::RegisterMesh(const std::string& tag, const std::vector<float>& vertices,
    const std::vector<unsigned int>& indices)
{
    if (meshMap.find(tag) != meshMap.end())
    {
        SNAKE_WRN("Mesh with tag \"" << tag << "\" already registered.");
        return;
    }
    meshMap[tag] = std::make_unique<Mesh>(vertices, indices);
}

void RenderManager::RegisterMaterial(const std::string& tag, const std::string& shaderTag,
    const std::unordered_map<std::string, std::string>& textureBindings)
{
    if (materialMap.find(tag) != materialMap.end())
    {
        SNAKE_WRN("Material tag already registered: " << tag);
        return;
    }

    Shader* shader = shaderMap[shaderTag].get();
    if (!shader)
    {
        SNAKE_WRN("Shader not found: " << shaderTag);
        return;
    }

    auto material = std::make_unique<Material>(shader);

    for (const auto& [uniformName, textureTag] : textureBindings)
    {
        auto it = textureMap.find(textureTag);
        if (it != textureMap.end())
            material->SetTexture(uniformName, it->second.get());
        else
            SNAKE_WRN("Texture not found: " << textureTag);
    }

    materialMap[tag] = std::move(material);
}
