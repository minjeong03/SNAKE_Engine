#include "Player.h"
#include <iostream>
#include "Bullet.h"
#include "Debug.h"
#include "Engine.h"

void Player::Init(const EngineContext& engineContext)
{
	pos = glm::vec2(0, 0);
//	SetMaterial(engineContext.renderManager->CreateMaterial("default", {}));
//	GetMaterial()->SetUniform("u_Model", glm::mat4(0));
}

void Player::LateInit(const EngineContext& engineContext)
{

}

void Player::Update(float dt, const EngineContext& engineContext)
{
	if (engineContext.inputManager->IsKeyDown(KEY_W))
	{
		pos += glm::vec2(0, 1 * dt);
		SNAKE_LOG("player pos: " << pos.x << " " << pos.y);
	}
	if (engineContext.inputManager->IsKeyDown(KEY_A))
	{
		pos += glm::vec2(-1 * dt, 0);
		SNAKE_LOG("player pos: " << pos.x << " " << pos.y);
	}
	if (engineContext.inputManager->IsKeyDown(KEY_S))
	{
		pos += glm::vec2(0, -1 * dt);
		SNAKE_LOG("player pos: " << pos.x << " " << pos.y);
	}
	if (engineContext.inputManager->IsKeyDown(KEY_D))
	{
		pos += glm::vec2(1 * dt, 0);
		SNAKE_LOG("player pos: " << pos.x << " " << pos.y);
	}

	if (engineContext.inputManager->IsKeyPressed(KEY_SPACE))
	{
		SNAKE_LOG("player shot the bullet");
		engineContext.stateManager->GetCurrentState()->GetObjectManager().AddObject(std::make_unique<Bullet>(pos));
	}
}

void Player::Draw(const EngineContext& engineContext)
{
}

void Player::Free(const EngineContext& engineContext)
{
	SNAKE_LOG("Player Free Called");
}

void Player::LateFree(const EngineContext& engineContext)
{
	SNAKE_LOG("Player LateFree Called");
}
