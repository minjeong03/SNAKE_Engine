#pragma once
#include <memory>
#include <string>

#include "Animation.h"
#include "Collider.h"
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

    virtual void OnCollision(Object* other) {}

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

    [[nodiscard]] glm::mat4 GetTransform2DMatrix();

    [[nodiscard]] Transform2D& GetTransform2D();

    void SetColor(const  glm::vec4& color_);

    [[nodiscard]] const glm::vec4& GetColor();

    virtual bool HasAnimation() const { return spriteAnimator != nullptr; }

    virtual SpriteAnimator* GetAnimator() { return spriteAnimator.get(); }

    void AttachAnimator(std::unique_ptr<SpriteAnimator> anim) { spriteAnimator = std::move(anim); }

    void AttachAnimator(SpriteSheet* sheet, float frameTime, bool loop = true) { spriteAnimator = std::make_unique<SpriteAnimator>(sheet, frameTime, loop); }

    void SetCollider(std::unique_ptr<Collider> c) { collider = std::move(c); }

    Collider* GetCollider() const { return collider.get(); }

    void SetCollision(ObjectManager& objectManager, const std::string& tag, const std::vector<std::string>& checkCollisionList);

    uint32_t GetCollisionMask() const { return collisionMask; }

    uint32_t GetCollisionCategory() const { return collisionCategory; }

    [[nodiscard]] bool ShouldIgnoreCamera() const;

    void SetIgnoreCamera(bool shouldIgnoreCamera, Camera2D* cameraForTransformCalc = nullptr);

    [[nodiscard]] ObjectType GetType() const { return type; }

    [[nodiscard]] Camera2D* GetReferenceCamera() const { return referenceCamera; }

    virtual glm::vec2 GetWorldPosition() const;

    virtual glm::vec2 GetWorldScale() const;

protected:
    Object(ObjectType objectType) : type(objectType) {}

    bool isAlive = true;
    bool isVisible = true;

    bool ignoreCamera = false;
    Camera2D* referenceCamera = nullptr;

    std::string objectTag;

    Transform2D transform2D;
    [[nodiscard]] virtual float GetBoundingRadius() const;
    uint8_t renderLayer = 0;
    Material* material = nullptr;
    Mesh* mesh = nullptr;
    std::unique_ptr<SpriteAnimator> spriteAnimator;
    std::unique_ptr<Collider> collider;
    glm::vec4 color = glm::vec4(1);
    ObjectType type;

    uint32_t collisionCategory = 0;
    uint32_t collisionMask = 0;
};
