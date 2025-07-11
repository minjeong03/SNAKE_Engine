#include "Bullet.h"

#include <iostream>

#include "Debug.h"
#include "Engine.h"

Bullet::Bullet(glm::vec2 _pos):pos(_pos)
{

}

void Bullet::Init(const EngineContext& engineContext)
{
	SNAKE_LOG("Bullet initialized");
}

void Bullet::LateInit(const EngineContext& engineContext)
{
}

void Bullet::Update(float dt, const EngineContext& engineContext)
{
	pos += glm::vec2(1*dt, 1*dt);
	SNAKE_LOG("Bullet pos: " << pos.x << " " << pos.y );
	timer += dt;
	if (timer>2.f)
	{
		Kill();
	}
}

void Bullet::Draw(const EngineContext& engineContext)
{

}

void Bullet::Free(const EngineContext& engineContext)
{
	SNAKE_LOG("Bullet Free Called");
}

void Bullet::LateFree(const EngineContext& engineContext)
{
	SNAKE_LOG("Bullet LateFree Called");
}
