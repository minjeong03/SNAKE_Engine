#pragma once
#include "Mesh.h"
#include "Transform.h"

struct EngineContext;
class FrustumCuller;

class GameObject
{
    friend FrustumCuller;
public:
    virtual void Init(const EngineContext& engineContext) = 0;

    virtual void LateInit(const EngineContext& engineContext) = 0;

    virtual void Update(float dt, const EngineContext& engineContext) = 0;

    virtual void Draw(const EngineContext& engineContext) = 0;

    virtual void Free(const EngineContext& engineContext) = 0;

    virtual void LateFree(const EngineContext& engineContext) = 0;

    virtual ~GameObject() = default;

    [[nodiscard]] const bool& IsAlive() const;

    [[nodiscard]] const bool& IsVisible() const;

    void SetVisibility(bool _isVisible);

    void Kill();

    void SetTag(const std::string& tag);

    [[nodiscard]] const std::string& GetTag() const;

    [[nodiscard]] const uint8_t& GetRenderLayer() const;

    void SetRenderLayer(const EngineContext& engineContext, const std::string& tag);

    void SetMaterial(const EngineContext& engineContext, const std::string& tag);

    [[nodiscard]] Material* GetMaterial() const;

    void SetMesh(const EngineContext& engineContext, const std::string& tag);

    [[nodiscard]] Mesh* GetMesh() const;

    [[nodiscard]] bool CanBeInstanced() const;

    [[nodiscard]] glm::mat4& GetTransform2DMatrix();

    [[nodiscard]] Transform2D& GetTransform2D();

protected:
    bool isAlive = true;
    bool isVisible = true;

    std::string objectTag;

    Transform2D transform2D;

private:
    uint8_t renderLayer = 0;
    Material* material = nullptr;
    Mesh* mesh = nullptr;

    [[nodiscard]] float GetBoundingRadius() const;
};
