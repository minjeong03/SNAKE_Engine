#include "RenderManager.h"
#include <algorithm>
#include <iostream>

#include "Camera2D.h"
#include "../ThirdParty/glad/gl.h"

#include "Debug.h"
#include"GameObject.h"
#include "Material.h"
#include "InstanceBatchKey.h"
#include "EngineContext.h"
#include "WindowManager.h"

void RenderManager::ClearDrawCommands()
{
    renderQueue.clear();
}

void RenderManager::Submit(std::function<void()>&& drawFunc)
{
    renderQueue.push_back({ std::move(drawFunc)});
}

void RenderManager::Submit(const EngineContext& engineContext, const std::vector<GameObject*>& objects, Camera2D* camera)
{
    std::vector<GameObject*> visibleObjects;
    FrustumCuller::CullVisible(*camera, objects, visibleObjects, glm::vec2(engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight()));
    RenderMap renderMap = BuildRenderMap(visibleObjects);
    SubmitRenderMap(engineContext, camera, renderMap);
}

void RenderManager::FlushDrawCommands()
{
    for (const auto& cmd : renderQueue)
    {
        cmd();
    }
    renderQueue.clear();
}

void RenderManager::SetViewport(int x, int y, int width, int height)
{
    Submit([=]() {
        glViewport(x, y, width, height);
        });
}

void RenderManager::ClearBackground(int x, int y, int width, int height, glm::vec4 color)
{
    Submit([=]() {
        glEnable(GL_SCISSOR_TEST);
        glScissor(x, y, width, height);
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);
        });
}

RenderLayerManager& RenderManager::GetRenderLayerManager()
{
    return renderLayerManager;
}

RenderMap RenderManager::BuildRenderMap(const std::vector<GameObject*>& source)
{
    RenderMap renderMap;

    for (auto* obj : source)
    {
        if (!obj || !obj->IsVisible())
            continue;

        Material* material = obj->GetMaterial();
        Mesh* mesh = obj->GetMesh();
        Shader* shader = material ? material->GetShader() : nullptr;

        if (!material || !mesh || !shader)
            continue;

        uint8_t layer = obj->GetRenderLayer();
        if (layer >= RenderLayerManager::MAX_LAYERS)
        {
            SNAKE_WRN("render skipped - invalid layer\n");
            continue;
        }

        InstanceBatchKey key{ mesh, material };
        renderMap[layer][shader][key].push_back(obj);
    }

    return renderMap;
}

void RenderManager::SubmitRenderMap(const EngineContext& engineContext, Camera2D* camera, const RenderMap& renderMap)
{
    Material* lastMaterial = nullptr;
    Shader* lastShader = nullptr;

    for (uint8_t layer = 0; layer < renderMap.size(); ++layer)
    {
        const ShaderMap& _shaderMap = renderMap[layer];

        for (const auto& [shader, batchMap] : _shaderMap)
        {
            for (const auto& [key, batch] : batchMap)
            {
                if (batch.front()->CanBeInstanced())
                {
                    Submit([=]() mutable {
                        std::vector<glm::mat4> transforms;
                        transforms.reserve(batch.size());
                        for (auto* obj : batch)
                            transforms.push_back(obj->GetTransform2DMatrix());

                        Material* material = key.material;
                        Shader* currentShader = material->GetShader();

                        if (material != lastMaterial)
                        {
                            material->Bind();
                            lastMaterial = material;
                        }

                        if (currentShader != lastShader)
                        {
                            material->SetUniform("u_Projection", camera->GetProjectionMatrix());
                            lastShader = currentShader;
                        }

                        batch.front()->Draw(engineContext);
                        material->SendUniforms();

                        key.mesh->BindVAO();
                        material->UpdateInstanceBuffer(transforms);
                        key.mesh->DrawInstanced(static_cast<GLsizei>(transforms.size()));
                        material->UnBind();
                        });
                }
                else
                {
                    for (auto* obj : batch)
                    {
                        Submit([=]() mutable {
                            Material* mat = key.material;
                            Shader* currentShader = mat->GetShader();

                            if (mat != lastMaterial)
                            {
                                mat->Bind();
                                lastMaterial = mat;
                            }

                            if (currentShader != lastShader)
                            {
                                mat->SetUniform("u_Projection", camera->GetProjectionMatrix());
                                lastShader = currentShader;
                            }

                            obj->Draw(engineContext);
                            mat->SendUniforms();
                            key.mesh->Draw();
                            mat->UnBind();
                            });
                    }
                }
            }
        }
    }
}

/*
 * Usage:
 * renderManager.RegisterShader("basic", {
 * { ShaderStage::Vertex, "shaders/basic.vert" },
 * { ShaderStage::Fragment, "shaders/basic.frag" }
 * });
 */
void RenderManager::RegisterShader(const std::string& tag, const std::vector<std::pair<ShaderStage, FilePath>>& sources)
{
    if (shaderMap.find(tag) != shaderMap.end())
    {
        SNAKE_WRN("Shader with tag \"" << tag << "\" already registered.");
        return;
    }
    auto shader = std::make_unique<Shader>();

    for (const auto& [stage, path] : sources)
        shader->AttachFromFile(stage, path);

    shader->Link();
    shaderMap[tag] = std::move(shader);
}

void RenderManager::RegisterShader(const std::string& tag, std::unique_ptr<Shader> shader)
{
    if (shaderMap.find(tag) != shaderMap.end())
    {
        SNAKE_WRN("Shader with tag \"" << tag << "\" already registered.");
        return;
    }
    shaderMap[tag] = std::move(shader);
}

void RenderManager::RegisterTexture(const std::string& tag, const FilePath& path, const TextureSettings& settings)
{
    if (textureMap.find(tag) != textureMap.end())
    {
        SNAKE_WRN("Texture with tag \"" << tag << "\" already registered.");
        return;
    }
	textureMap[tag] = std::make_unique<Texture>(path, settings);
}

void RenderManager::RegisterTexture(const std::string& tag, std::unique_ptr<Texture> texture)
{
    if (textureMap.find(tag) != textureMap.end())
    {
        SNAKE_WRN("Texture with tag \"" << tag << "\" already registered.");
        return;
    }
    textureMap[tag] = std::move(texture);
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

void RenderManager::RegisterMesh(const std::string& tag, std::unique_ptr<Mesh> mesh)
{
    if (meshMap.find(tag) != meshMap.end())
    {
        SNAKE_WRN("Mesh with tag \"" << tag << "\" already registered.");
        return;
    }
    meshMap[tag] = std::move(mesh);
}

void RenderManager::RegisterMaterial(const std::string& tag, const std::string& shaderTag,
                                     const std::unordered_map<UniformName, TextureTag>& textureBindings)
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

void RenderManager::RegisterMaterial(const std::string& tag, std::unique_ptr<Material> material)
{
    if (materialMap.find(tag) != materialMap.end())
    {
        SNAKE_WRN("Material tag already registered: " << tag);
        return;
    }
    materialMap[tag] = std::move(material);
}

void RenderManager::RegisterRenderLayer(const std::string& tag)
{
    renderLayerManager.RegisterLayer(tag);
}
