#include "Bullet.h"

#include <iostream>

#include "Debug.h"
#include "Engine.h"

Bullet::Bullet(glm::vec2 pos, glm::vec2 _dir): dir(_dir)
{
    transform2D.SetPosition(pos);
}

void Bullet::Init(const EngineContext& engineContext)
{
    SNAKE_LOG("Bullet initialized");
   
    mesh = engineContext.renderManager->GetMeshByTag("star");
    material = engineContext.renderManager->GetMaterialByTag("m_instancing");
    material->EnableInstancing(true, mesh);
   // renderLayer = 3;
    transform2D.SetScale(glm::vec2(50.f));
    objectTag = "bullet";
}

void Bullet::LateInit(const EngineContext& engineContext)
{
}

void Bullet::Update(float dt, const EngineContext& engineContext)
{
    transform2D.AddPosition(glm::vec2(1 * dir.x, 1 * dir.y));
    //SNAKE_LOG("Bullet pos: " << pos.x << " " << pos.y);
    timer += dt;
}

void Bullet::Draw(const EngineContext& engineContext)
{
    material->SetUniform("u_Color", glm::vec4(1.0, 1.0, 1.0, 1.0));
}

void Bullet::Free(const EngineContext& engineContext)
{
    SNAKE_LOG("Bullet Free Called");
}

void Bullet::LateFree(const EngineContext& engineContext)
{
    SNAKE_LOG("Bullet LateFree Called");
}

void Bullet::SetRenderLayer(int layer)
{
    renderLayer = layer;
}
