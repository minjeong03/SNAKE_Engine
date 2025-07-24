#pragma once
#include "EngineContext.h"
#include "Mesh.h"
#include "Object.h"
#include "Transform.h"

class Camera2D;

struct TextInstance
{
    Font* font = nullptr;
    std::string text = "";
    std::string GetCacheKey() const
    {
        return "|" + text;
    }
};
class TextObject : public Object
{
public:
    TextObject(Font* font, const std::string& text);

    ~TextObject() override = default;

    void Init([[maybe_unused]] const EngineContext& engineContext) override;

    void LateInit([[maybe_unused]] const EngineContext& engineContext) override;
    
    void Update([[maybe_unused]] float dt, [[maybe_unused]] const EngineContext& engineContext) override;
    
    void Draw([[maybe_unused]] const EngineContext& engineContext) override;
    
    void Free([[maybe_unused]] const EngineContext& engineContext) override;
    
    void LateFree([[maybe_unused]] const EngineContext& engineContext) override;

    void SetText(const std::string& text);

    void SetTextInstance(const TextInstance& textInstance_);

    TextInstance* GetTextInstance();

    void SetMaterial(const EngineContext& engineContext, const std::string& tag) = delete;

    [[nodiscard]] Material* GetMaterial() const = delete;

    void SetMesh(const EngineContext& engineContext, const std::string& tag) = delete;

    [[nodiscard]] Mesh* GetMesh() const = delete;

protected:
    TextInstance textInstance;
    std::unordered_map<std::string, std::unique_ptr<Mesh>> textMeshCache;
};
