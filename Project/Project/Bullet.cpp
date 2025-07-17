#include "Bullet.h"

#include <iostream>
#include <random>

#include "Debug.h"
#include "Engine.h"

Bullet::Bullet(glm::vec2 pos, glm::vec2 _dir) : dir(_dir)
{
    transform2D.SetPosition(pos);
}

void Bullet::Init(const EngineContext& engineContext)
{
    SNAKE_LOG("Bullet initialized");
    SetMesh(engineContext, "star");
    SetMaterial(engineContext, "m_instancing");
    SetRenderLayer(engineContext, "Bullet");
    GetMaterial()->EnableInstancing(true, GetMesh());
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> scaleDist(10.0f, 40.0f);
    float scale = scaleDist(gen);
    transform2D.SetScale(glm::vec2(scale));
    objectTag = "bullet";
}

void Bullet::LateInit(const EngineContext& engineContext)
{
}

void Bullet::Update(float dt, const EngineContext& engineContext)
{
    transform2D.SetRotation(transform2D.GetRotation() + dt * 2);
    transform2D.AddPosition(glm::vec2(1 * dir.x, 1 * dir.y));
    SNAKE_LOG("Bullet pos: " << transform2D.GetPosition().x << " " << transform2D.GetPosition().y);
    timer += dt;
}

void Bullet::Draw(const EngineContext& engineContext)
{
    GetMaterial()->SetUniform("u_Color", glm::vec4(1.0, 1.0, 1.0, 1.0));
}

void Bullet::Free(const EngineContext& engineContext)
{
    SNAKE_LOG("Bullet Free Called");
}

void Bullet::LateFree(const EngineContext& engineContext)
{
    SNAKE_LOG("Bullet LateFree Called");
}
