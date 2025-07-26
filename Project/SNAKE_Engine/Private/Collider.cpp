#include "Collider.h"
#include "Object.h"
#include "RenderManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/norm.hpp"

float CircleCollider::GetRadius() const
{
    return useTransformScale ? baseRadius * std::max(glm::abs(owner->GetTransform2D().GetScale().x), glm::abs(owner->GetTransform2D().GetScale().y)): scaledRadius;
}

void CircleCollider::SetRadius(float r)
{
    baseRadius = r;

    if (!useTransformScale)
        scaledRadius = r;
}

float CircleCollider::GetBoundingRadius() const
{
    return GetRadius();
}

bool CircleCollider::CheckCollision(const Collider* other) const
{
    return other->DispatchAgainst(*this);
}

bool CircleCollider::DispatchAgainst(const CircleCollider& other) const
{
    glm::vec2 a = owner->GetTransform2D().GetPosition();
    glm::vec2 b = other.GetOwner()->GetTransform2D().GetPosition();

    float distSqr = glm::length2(a - b);
    float rSum = GetRadius() + other.GetRadius();
    return distSqr <= rSum * rSum;
}

bool CircleCollider::DispatchAgainst(const AABBCollider& other) const
{
    return other.DispatchAgainst(*this);
}

void CircleCollider::SyncWithTransformScale()
{
    if (!useTransformScale) return;

    float scale = std::max(glm::abs(owner->GetTransform2D().GetScale().x),glm::abs(owner->GetTransform2D().GetScale().y));
    scaledRadius = baseRadius * scale;
}

bool CircleCollider::CheckPointCollision(const glm::vec2& point) const
{
    glm::vec2 center = owner->GetTransform2D().GetPosition();
    float distSqr = glm::dot(center - point, center - point);
    return distSqr <= GetRadius() * GetRadius();
}

void CircleCollider::DrawDebug(RenderManager* rm, Camera2D* cam, const glm::vec4& color) const
{
    glm::vec2 center = owner->GetTransform2D().GetPosition();
    float r = GetRadius();
    const int segments = 20;
    const float angleStep = glm::two_pi<float>() / segments;

    for (int i = 0; i < segments; ++i)
    {
        float angleA = i * angleStep;
        float angleB = (i + 1) * angleStep;

        glm::vec2 a = center + r * glm::vec2(std::cos(angleA), std::sin(angleA));
        glm::vec2 b = center + r * glm::vec2(std::cos(angleB), std::sin(angleB));

        rm->DrawDebugLine(a, b, cam, color);
    }
}


glm::vec2 AABBCollider::GetHalfSize() const
{
    return useTransformScale ? baseHalfSize * glm::abs(owner->GetTransform2D().GetScale()) : scaledHalfSize;
}

void AABBCollider::SetHalfSize(const glm::vec2& halfSize)
{
    baseHalfSize = halfSize;

    if (!useTransformScale)
        scaledHalfSize = halfSize;
}

float AABBCollider::GetBoundingRadius() const
{
    glm::vec2 half = GetHalfSize();
    return glm::length(half);
}


bool AABBCollider::CheckCollision(const Collider* other) const
{
    return other->DispatchAgainst(*this);
}

bool AABBCollider::CheckPointCollision(const glm::vec2& point) const
{
    glm::vec2 center = owner->GetTransform2D().GetPosition();
    glm::vec2 half = GetHalfSize();

    glm::vec2 min = center - half;
    glm::vec2 max = center + half;

    return point.x >= min.x && point.x <= max.x &&
        point.y >= min.y && point.y <= max.y;
}

bool AABBCollider::DispatchAgainst(const AABBCollider& other) const
{
    glm::vec2 aPos = owner->GetTransform2D().GetPosition();
    glm::vec2 bPos = other.GetOwner()->GetTransform2D().GetPosition();

    glm::vec2 aHalf = GetHalfSize();
    glm::vec2 bHalf = other.GetHalfSize();

    return std::abs(aPos.x - bPos.x) <= (aHalf.x + bHalf.x) &&
        std::abs(aPos.y - bPos.y) <= (aHalf.y + bHalf.y);
}

void AABBCollider::SyncWithTransformScale()
{
    if (useTransformScale)
        scaledHalfSize = baseHalfSize * glm::abs(owner->GetTransform2D().GetScale());
}

void AABBCollider::DrawDebug(RenderManager* rm, Camera2D* cam, const glm::vec4& color) const
{
    glm::vec2 center = owner->GetTransform2D().GetPosition();
    glm::vec2 half = GetHalfSize();

    glm::vec2 min = center - half;
    glm::vec2 max = center + half;

    rm->DrawDebugLine({ min.x, min.y }, { max.x, min.y }, cam, color);
    rm->DrawDebugLine({ max.x, min.y }, { max.x, max.y }, cam, color);
    rm->DrawDebugLine({ max.x, max.y }, { min.x, max.y }, cam, color);
    rm->DrawDebugLine({ min.x, max.y }, { min.x, min.y }, cam, color);
}

bool AABBCollider::DispatchAgainst(const CircleCollider& other) const
{
    glm::vec2 aPos = owner->GetTransform2D().GetPosition();
    glm::vec2 half = GetHalfSize();

    glm::vec2 circlePos = other.GetOwner()->GetTransform2D().GetPosition();
    float radius = other.GetRadius();

    glm::vec2 closest = glm::clamp(circlePos, aPos - half, aPos + half);
    float distSqr = glm::length2(circlePos - closest);

    return distSqr <= radius * radius;
}



void SpatialHashGrid::Clear()
{
    grid.clear();
}

void SpatialHashGrid::Insert(Object* obj)
{
    if (!obj->IsAlive() || !obj->GetCollider())
        return;

    glm::vec2 pos = obj->GetTransform2D().GetPosition();
    float radius = obj->GetCollider()->GetBoundingRadius();

    glm::ivec2 minCell = GetCell(pos - glm::vec2(radius));
    glm::ivec2 maxCell = GetCell(pos + glm::vec2(radius));

    for (int y = minCell.y; y <= maxCell.y; ++y)
    {
        for (int x = minCell.x; x <= maxCell.x; ++x)
        {
            InsertToCell(obj, { x, y });
        }
    }
}

void SpatialHashGrid::ComputeCollisions(std::function<void(Object*, Object*)> onCollision)
{
    for (auto& [cell, list] : grid)
    {
        const size_t count = list.size();
        for (size_t i = 0; i < count; ++i)
        {
            for (size_t j = i + 1; j < count; ++j)
            {
                onCollision(list[i], list[j]);
            }
        }
    }
}

glm::ivec2 SpatialHashGrid::GetCell(const glm::vec2& pos) const
{
    return glm::ivec2(
        static_cast<int>(std::floor(pos.x / cellSize)),
        static_cast<int>(std::floor(pos.y / cellSize))
    );
}

void SpatialHashGrid::InsertToCell(Object* obj, const glm::ivec2& cell)
{
    grid[cell].push_back(obj);
}

uint32_t CollisionGroupRegistry::GetGroupBit(const std::string& tag)
{
    auto it = tagToBit.find(tag);
    if (it != tagToBit.end())
        return it->second;

    if (currentBit >= 32)
    {
        SNAKE_ERR("Exceeded maximum number of collision groups");
        return UINT32_MAX;
    }

    uint32_t bit = (1 << currentBit);
    tagToBit[tag] = bit;
    bitToTag[bit] = tag;
    ++currentBit;
    return bit;
}

std::string CollisionGroupRegistry::GetGroupTag(uint32_t bit) const
{
    auto it = bitToTag.find(bit);
    if (it != bitToTag.end())
        return it->second;
    return "unknown";
}
