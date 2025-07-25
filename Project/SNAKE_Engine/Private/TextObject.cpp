#include "TextObject.h"
#include "EngineContext.h"
#include "RenderManager.h"

TextObject::TextObject(Font* font, const std::string& text, TextAlignH alignH_, TextAlignV alignV_) : Object(ObjectType::TEXT)
{

    std::string cacheKey = textInstance.GetCacheKey();

    alignH = alignH_;
    alignV = alignV_;

    textInstance.font = font;
    textInstance.text = text;
    material = font->GetMaterial();
    mesh = nullptr;

    UpdateMesh();
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
    material->SetUniform("u_Color", color);
}

void TextObject::Free(const EngineContext& engineContext)
{
}

void TextObject::LateFree(const EngineContext& engineContext)
{
}

float TextObject::GetBoundingRadius() const
{
    if (!mesh) return 0.0f;
    glm::vec2 size = textInstance.font->GetTextSize(textInstance.text); 
    glm::vec2 scaled = size * transform2D.GetScale();
    return glm::length(scaled) ;
}


void TextObject::SetText(const std::string& text)
{
    if (textInstance.text == text)
        return;

    textInstance.text = text;

    UpdateMesh();
}

void TextObject::SetTextInstance(const TextInstance& textInstance_)
{
    textInstance = textInstance_;
}

void TextObject::SetAlignH(TextAlignH alignH_)
{
    if (alignH == alignH_)
        return;

    alignH = alignH_;

    UpdateMesh();
}

void TextObject::SetAlignV(TextAlignV alignV_)
{
    if (alignV == alignV_)
        return;

    alignV = alignV_;

    UpdateMesh();
}

TextInstance* TextObject::GetTextInstance()
{
    return &textInstance;
}

void TextObject::UpdateMesh()
{
    if (textMeshCache.size() < 500)
        textMeshCache.clear();

    auto it = textMeshCache.find(textInstance.GetCacheKey());
    if (it != textMeshCache.end())
    {
        mesh = it->second.get();
    }
    else
    {
        std::unique_ptr<Mesh> newMesh(textInstance.font->GenerateTextMesh(textInstance.text, alignH, alignV));
        mesh = newMesh.get();
        textMeshCache[textInstance.GetCacheKey()] = std::move(newMesh);
    }
}
