#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "glm.hpp"


class SpatialHashGrid;
class ObjectManager;
class Camera2D;
class RenderManager;
class Object;
class CircleCollider;
class AABBCollider;

enum class ColliderType
{
    None,
    Circle,
    AABB
};

class Collider
{
    friend ObjectManager;
    friend CircleCollider;
    friend AABBCollider;
    friend SpatialHashGrid;
public:
    Collider() = delete;
    Collider(Object* owner_) : owner(owner_), worldPosition(){}
    virtual ~Collider() = default;

    void SetUseTransformScale(bool use) { useTransformScale = use; }
    [[nodiscard]] bool IsUsingTransformScale() const { return useTransformScale; }

    void SetWorldPosition(const glm::vec2& pos) { worldPosition = pos; }
    const glm::vec2& GetWorldPosition() const { return worldPosition; }

    virtual bool CheckPointCollision(const glm::vec2& point) const = 0;

protected:
    [[nodiscard]] Object* GetOwner() const { return owner; }

    virtual ColliderType GetType() const = 0;

    virtual float GetBoundingRadius() const = 0;

    virtual bool CheckCollision(const Collider* other) const = 0;

    virtual bool DispatchAgainst(const CircleCollider& other) const = 0;
    virtual bool DispatchAgainst(const AABBCollider& other) const = 0;

    virtual void SyncWithTransformScale() = 0;

    virtual void DrawDebug(RenderManager* rm, Camera2D* cam, const glm::vec4& color = { 1,0,0,1 }) const = 0;

    Object* owner;
    bool useTransformScale = true;
    glm::vec2 worldPosition;
};


class CircleCollider : public Collider
{
    friend AABBCollider;
    friend SpatialHashGrid;
public:
    CircleCollider(Object* owner, float size)
        : Collider(owner), baseRadius(size/2.f), scaledRadius(size/2.f) {
    }

    [[nodiscard]] float GetRadius() const;

    [[nodiscard]] float GetSize() const;

    void SetRadius(float r);

    bool CheckPointCollision(const glm::vec2& point) const override;

private:
    ColliderType GetType() const override { return ColliderType::Circle; }

    float GetBoundingRadius() const override;

    bool CheckCollision(const Collider* other) const override;
    
    bool DispatchAgainst(const CircleCollider& other) const override;
    bool DispatchAgainst(const AABBCollider& other) const override;

    void SyncWithTransformScale() override;

    void DrawDebug(RenderManager* rm, Camera2D* cam, const glm::vec4& color) const override;

    float baseRadius = 0.5f;
    float scaledRadius = 0.5f;
};


class AABBCollider : public Collider
{
    friend CircleCollider;
    friend SpatialHashGrid;
public:
    AABBCollider(Object* owner, const glm::vec2& size)
        : Collider(owner), baseHalfSize(size/glm::vec2(2)), scaledHalfSize(size / glm::vec2(2)) {
    }

    [[nodiscard]] glm::vec2 GetHalfSize() const;

    [[nodiscard]] glm::vec2 GetSize() const;
    void SetSize(const glm::vec2& hs);

    bool CheckPointCollision(const glm::vec2& point) const override;

private:
    ColliderType GetType() const override { return ColliderType::AABB; }

    float GetBoundingRadius() const override;

    bool CheckCollision(const Collider* other) const override;\

    bool DispatchAgainst(const CircleCollider& other) const override;
    bool DispatchAgainst(const AABBCollider& other) const override;

    void SyncWithTransformScale() override;

    void DrawDebug(RenderManager* rm, Camera2D* cam, const glm::vec4& color) const override;

    glm::vec2 baseHalfSize = { 0.5f, 0.5f };
    glm::vec2 scaledHalfSize = { 0.5f, 0.5f };
};

struct Vec2Hash
{
    size_t operator()(const glm::ivec2& v) const
    {
        return std::hash<int>()(v.x * 73856093 ^ v.y * 19349663);
    }
};

class SpatialHashGrid
{
public:
    void Clear();
    void Insert(Object* obj);
    void ComputeCollisions(std::function<void(Object*, Object*)> onCollision);

private:
    glm::ivec2 GetCell(const glm::vec2& pos) const;
    void InsertToCell(Object* obj, const glm::ivec2& cell);

    int cellSize = 50; 
    std::unordered_map<glm::ivec2, std::vector<Object*>, Vec2Hash> grid;
};

class CollisionGroupRegistry
{
public:
    uint32_t GetGroupBit(const std::string& tag);
    std::string GetGroupTag(uint32_t bit) const;

private:
    std::unordered_map<std::string, uint32_t> tagToBit;
    std::unordered_map<uint32_t, std::string> bitToTag;
    uint32_t currentBit = 0;
};