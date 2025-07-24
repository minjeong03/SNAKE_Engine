#include "TextObject.h"
#include "EngineContext.h"
#include "RenderManager.h"

TextObject::TextObject(Font* font, const std::string& text): Object(ObjectType::TEXT)
{

    std::string cacheKey = textInstance.GetCacheKey();

    textInstance.font = font;
    textInstance.text = text;
    material = font->GetMaterial();
    mesh = nullptr;
    auto it = textMeshCache.find(textInstance.GetCacheKey());
    if (it != textMeshCache.end())
    {
        mesh = it->second.get();
    }
    else
    {
        std::unique_ptr<Mesh> newMesh(font->GenerateTextMesh(textInstance.text));
        mesh = newMesh.get();
        textMeshCache[cacheKey] = std::move(newMesh);
    }
}

void TextObject::Init(const EngineContext& engineContext)
{
}

void TextObject::LateInit(const EngineContext& engineContext)
{
}

void TextObject::Update(float dt, const EngineContext& engineContext)
{
}

void TextObject::Draw(const EngineContext& engineContext)
{
    material->SetUniform("u_Color", glm::vec4(1));
}

void TextObject::Free(const EngineContext& engineContext)
{
}

void TextObject::LateFree(const EngineContext& engineContext)
{
}

void TextObject::SetText(const std::string& text)
{
    if (textInstance.text == text)
        return;

    textInstance.text = text;

    if (textMeshCache.size() < 500)
        textMeshCache.clear();

    auto it = textMeshCache.find(textInstance.GetCacheKey());
    if (it != textMeshCache.end())
    {
        mesh = it->second.get();
    }
    else
    {
        std::unique_ptr<Mesh> newMesh(textInstance.font->GenerateTextMesh(textInstance.text));
        mesh = newMesh.get();
        textMeshCache[textInstance.GetCacheKey()] = std::move(newMesh);
    }

}

void TextObject::SetTextInstance(const TextInstance& textInstance_)
{
    textInstance = textInstance_;
}

TextInstance* TextObject::GetTextInstance()
{
    return &textInstance;
}
