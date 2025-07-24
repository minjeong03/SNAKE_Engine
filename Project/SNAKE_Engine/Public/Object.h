#pragma once
#include <string>

#include "Mesh.h"
#include "Transform.h"
class FrustumCuller;
struct EngineContext;
enum class ObjectType
{
    GAME,
    TEXT
};
class Object
{
    friend FrustumCuller;
public:
    Object() = delete;
    virtual void Init([[maybe_unused]] const EngineContext& engineContext) = 0;

    virtual void LateInit([[maybe_unused]] const EngineContext& engineContext) = 0;

    virtual void Update([[maybe_unused]] float dt, [[maybe_unused]] const EngineContext& engineContext) = 0;

    virtual void Draw([[maybe_unused]] const EngineContext& engineContext) = 0;

    virtual void Free([[maybe_unused]] const EngineContext& engineContext) = 0;

    virtual void LateFree([[maybe_unused]] const EngineContext& engineContext) = 0;

    virtual ~Object() = default;

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

    ObjectType GetType() const { return type; }

protected:
    Object(ObjectType objectType) : type(objectType) {}

    bool isAlive = true;
    bool isVisible = true;

    std::string objectTag;

    Transform2D transform2D;
    [[nodiscard]] float GetBoundingRadius() const;
    uint8_t renderLayer = 0;
    Material* material = nullptr;
    Mesh* mesh = nullptr;

    ObjectType type;
};
