#include "Bullet.h"

#include <iostream>

#include "Engine.h"

Bullet::Bullet(glm::vec2 _pos):pos(_pos)
{

}

void Bullet::Init()
{
	std::cout << "Bullet initialized" << std::endl;
}

void Bullet::LateInit()
{
}

void Bullet::Update(float dt, const EngineContext& engineContext)
{
	pos += glm::vec2(1*dt, 1*dt);
	std::cout << "Bullet pos: " << pos.x << " " << pos.y << std::endl;
	timer += dt;
	if (timer>2.f)
	{
		Kill();
	}
}

void Bullet::Draw(const EngineContext& engineContext)
{
	//std::cout << "Bullet draw\n";
}

void Bullet::Free()
{
	std::cout << "Bullet Free Called\n";
}

void Bullet::LateFree()
{
	std::cout << "Bullet LateFree Called\n";
}
