#include "Bullet.h"

#include <iostream>

#include "Debug.h"
#include "Engine.h"

Bullet::Bullet(glm::vec2 _pos, glm::vec2 _dir) :pos(_pos), dir(_dir)
{
}

void Bullet::Init(const EngineContext& engineContext)
{
    SNAKE_LOG("Bullet initialized");
    material = engineContext.renderManager->CreateMaterial("default", { std::pair<std::string, std::string>("u_Texture","default") });
    mesh = engineContext.renderManager->GetMeshByTag("star");
}

void Bullet::LateInit(const EngineContext& engineContext)
{
}

void Bullet::Update(float dt, const EngineContext& engineContext)
{
    pos += glm::vec2(1 * dir.x, 1 * dir.y);
    //SNAKE_LOG("Bullet pos: " << pos.x << " " << pos.y);
    timer += dt;
    if (timer > 10.f)
    {
		Kill();
    }
}

void Bullet::Draw(const EngineContext& engineContext)
{
    material->SetUniform("u_Color", glm::vec4(1.0, 1.0, 0, 1.0));
    material->SetUniform("u_Model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f)),glm::vec3(25)));
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
