#include "Bullet.h"

#include <iostream>

#include "Debug.h"
#include "Engine.h"

Bullet::Bullet(glm::vec2 pos, glm::vec2 _dir): dir(_dir)
{
    transform2D.position = pos;
}

void Bullet::Init(const EngineContext& engineContext)
{
    SNAKE_LOG("Bullet initialized");
   
    mesh = engineContext.renderManager->GetMeshByTag("star");
    material = engineContext.renderManager->GetMaterialByTag("m_instancing");
    material->EnableInstancing(true, mesh);
    renderLayer = 1;
    transform2D.scale = glm::vec2(50.f);
}

void Bullet::LateInit(const EngineContext& engineContext)
{


}

void Bullet::Update(float dt, const EngineContext& engineContext)
{
    transform2D.position += glm::vec2(1 * dir.x, 1 * dir.y);
    //SNAKE_LOG("Bullet pos: " << pos.x << " " << pos.y);
    timer += dt;

}

void Bullet::Draw(const EngineContext& engineContext)
{
    material->SetUniform("u_Color", glm::vec4(1.0, 1.0, 1.0, 1.0));
    material->SetUniform("u_Projection", glm::ortho(
        -engineContext.windowManager->GetWidth() / 2.0f, engineContext.windowManager->GetWidth() / 2.0f,
        -engineContext.windowManager->GetHeight() / 2.0f, engineContext.windowManager->GetHeight() / 2.0f
    ));
}

void Bullet::Free(const EngineContext& engineContext)
{
    SNAKE_LOG("Bullet Free Called");
}

void Bullet::LateFree(const EngineContext& engineContext)
{
    SNAKE_LOG("Bullet LateFree Called");
}
