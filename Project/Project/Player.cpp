#include "Player.h"

#include <iostream>

#include "Bullet.h"
#include "Engine.h"

void Player::Init()
{
	pos = glm::vec2(0, 0);
}

void Player::Update(float dt, const EngineContext& engineContext)
{
	if (engineContext.inputManager->IsKeyDown(KEY_W))
	{
		pos += glm::vec2(0, 1 * dt);
		std::cout << "player pos: " << pos.x << " " << pos.y << std::endl;
	}
	if (engineContext.inputManager->IsKeyDown(KEY_A))
	{
		pos += glm::vec2(-1 * dt, 0);
		std::cout << "player pos: " << pos.x << " " << pos.y << std::endl;
	}
	if (engineContext.inputManager->IsKeyDown(KEY_S))
	{
		pos += glm::vec2(0, -1 * dt);
		std::cout << "player pos: " << pos.x << " " << pos.y << std::endl;
	}
	if (engineContext.inputManager->IsKeyDown(KEY_D))
	{
		pos += glm::vec2(1 * dt, 0);
		std::cout << "player pos: " << pos.x << " " << pos.y << std::endl;
	}

	if (engineContext.inputManager->IsKeyPressed(KEY_SPACE))
	{
		std::cout << "player shot the bullet" << std::endl;
		engineContext.stateManager->GetCurrentState()->GetObjectManager().AddObject(std::make_unique<Bullet>(pos));
	}
}

void Player::Draw(const EngineContext& engineContext)
{
}
