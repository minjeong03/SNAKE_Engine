#include "RenderManager.h"
#include <algorithm>
#include <iostream>
#include "ext/matrix_clip_space.hpp"
#include "ext/matrix_transform.hpp"

#include "Camera2D.h"
#include "gl.h"

#include "Debug.h"
#include"Object.h"
#include "Material.h"
#include "InstanceBatchKey.h"
#include "EngineContext.h"
#include "TextObject.h"
#include "WindowManager.h"


void RenderManager::Submit(std::function<void()>&& drawFunc)
{
    renderQueue.push_back({ std::move(drawFunc) });
}

void RenderManager::Submit(const EngineContext& engineContext, const std::vector<Object*>& objects, Camera2D* camera)
{
    std::vector<Object*> visibleObjects;
    if (camera)
    {
        FrustumCuller::CullVisible(*camera, objects, visibleObjects, glm::vec2(engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight()));
        BuildRenderMap(visibleObjects, camera);
    }
    else
    {
        BuildRenderMap(objects, camera);
    }
}

void FrustumCuller::CullVisible(const Camera2D& camera, const std::vector<Object*>& allObjects,
    std::vector<Object*>& outVisibleList, glm::vec2 viewportSize)
{
    outVisibleList.clear();
    for (Object* obj : allObjects)
    {
        if (!obj->IsAlive() && !obj->IsVisible())
            continue;

        const glm::vec2& pos = obj->GetTransform2D().GetPosition();
        float radius = obj->GetBoundingRadius();

        if (camera.IsInView(pos, radius, viewportSize / camera.GetZoom()))
            outVisibleList.push_back(obj);
    }
}

void RenderManager::FlushDrawCommands(const EngineContext& engineContext)
{
    SubmitRenderMap(engineContext);
    for (const auto& cmd : renderQueue)
    {
        cmd();
    }
    for (auto& shdrMap : renderMap)
    {
        shdrMap.clear();
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

void RenderManager::Init(const EngineContext& engineContext)
{
    auto shader = std::make_unique<Shader>();

    shader->AttachFromSource(ShaderStage::Vertex, R"(
		#version 330 core
		layout (location = 0) in vec2 aPos;
		layout (location = 1) in vec2 aUV;

		uniform mat4 u_Model;
		uniform mat4 u_Projection;

		out vec2 v_TexCoord;

		void main()
		{
		    v_TexCoord = aUV;
		    gl_Position = u_Projection * u_Model * vec4(aPos, 0.0, 1.0);
		}
)");
    shader->AttachFromSource(ShaderStage::Fragment, R"(
	    #version 330 core
	    in vec2 v_TexCoord;
	    out vec4 FragColor;

	    uniform sampler2D u_FontTexture;
	    uniform vec4 u_Color;

	    void main()
	    {
	        float alpha = texture(u_FontTexture, v_TexCoord).r;
	        FragColor = vec4(u_Color.rgb, alpha * u_Color.a);
	    }
)");

    shader->Link();
    shaderMap["internal_text"] = std::move(shader);
    RegisterMaterial("internal_text", "internal_text",{});

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderManager::BuildRenderMap(const std::vector<Object*>& source, Camera2D* camera)
{
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
        renderMap[layer][shader][key].emplace_back(obj, camera);
    }
}

void RenderManager::SubmitRenderMap(const EngineContext& engineContext)
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
                if (batch.front().first->CanBeInstanced())
                {
                    Submit([=]() mutable {
                        std::vector<glm::mat4> transforms;
                        transforms.reserve(batch.size());
                        for (const auto& [obj, camera] : batch)
                            transforms.push_back(dynamic_cast<GameObject*>(obj)->GetTransform2DMatrix());

                        Material* material = key.material;
                        Shader* currentShader = material->GetShader();

                        if (material != lastMaterial)
                        {
                            material->Bind();
                            lastMaterial = material;
                        }

                        if (currentShader != lastShader)
                        {
                            material->SetUniform("u_Projection", batch.front().second->GetProjectionMatrix());
                            lastShader = currentShader;
                        }

                        batch.front().first->Draw(engineContext);
                        material->SendUniforms();

                        key.mesh->BindVAO();
                        material->UpdateInstanceBuffer(transforms);
                        key.mesh->DrawInstanced(static_cast<GLsizei>(transforms.size()));
                        material->UnBind();
                        });
                }
                else
                {
                    for (const auto& [obj, camera] : batch)
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
                                glm::mat4 projection;
                                if (camera == nullptr)
                                {
                                    projection = glm::ortho(
                                        -static_cast<float>(engineContext.windowManager->GetWidth())/2,
                                        static_cast<float>(engineContext.windowManager->GetWidth())/2,
                                        -static_cast<float>(engineContext.windowManager->GetHeight())/2,
                                        static_cast<float>(engineContext.windowManager->GetHeight())/2
                                    );
                                }
                                else
                                    projection = camera->GetProjectionMatrix();
                                mat->SetUniform("u_Projection", projection);
                                mat->SetUniform("u_Model", obj->GetTransform2D().GetMatrix());
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

void RenderManager::RegisterFont(const std::string& tag, const std::string& ttfPath, uint32_t pixelSize)
{
    if (fontMap.find(tag) != fontMap.end())
    {
        SNAKE_WRN("Font tag already registered: " << tag);
        return;
    }
    auto font = std::make_unique<Font>(*this, ttfPath, pixelSize);

    fontMap[tag] = std::move(font);
}

void RenderManager::RegisterFont(const std::string& tag, std::unique_ptr<Font> font)
{
    if (fontMap.find(tag) != fontMap.end())
    {
        SNAKE_WRN("Font tag already registered: " << tag);
        return;
    }
    fontMap[tag] = std::move(font);
}

void RenderManager::RegisterRenderLayer(const std::string& tag)
{
    renderLayerManager.RegisterLayer(tag);
}
