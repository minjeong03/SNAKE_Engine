#pragma once
#include "EngineContext.h"
#include "Mesh.h"
#include "Object.h"
#include "Transform.h"

class Camera2D;

struct TextInstance
{
    Font* font;
    std::string text;
    std::string GetCacheKey() const
    {
        return "|" + text;
    }
};
class TextObject : public Object
{
public:
    TextObject(Font* font, std::string text);

    ~TextObject() override = default;

    void Init(const EngineContext& engineContext) override;

    void LateInit(const EngineContext& engineContext) override;
    
    void Update(float dt, const EngineContext& engineContext) override;
    
    void Draw(const EngineContext& engineContext) override;
    
    void Free(const EngineContext& engineContext) override;
    
    void LateFree(const EngineContext& engineContext) override;

    void SetText(const std::string text);

    void SetTextInstance(const TextInstance& textInstance_);

    TextInstance* GetTextInstance();

protected:
    TextInstance textInstance;
    std::unordered_map<std::string, std::unique_ptr<Mesh>> textMeshCache;
};
