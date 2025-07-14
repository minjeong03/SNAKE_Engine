#include "Player.h"
#include <iostream>
#include <random>

#include "Bullet.h"
#include "Debug.h"
#include "Engine.h"

void Player::Init(const EngineContext& engineContext)
{
    transform2D.position = glm::vec2(0, 0);
    
    mesh = engineContext.renderManager->GetMeshByTag("default");
    material = engineContext.renderManager->GetMaterialByTag("m_default");
    renderLayer = 0;
}

void Player::LateInit(const EngineContext& engineContext)
{

}

void Player::Update(float dt, const EngineContext& engineContext)
{
    if (engineContext.inputManager->IsKeyDown(KEY_W))
    {
        transform2D.position += glm::vec2(0, 150 * dt);
    }
    if (engineContext.inputManager->IsKeyDown(KEY_A))
    {
        transform2D.position += glm::vec2(-150 * dt, 0);
    }
    if (engineContext.inputManager->IsKeyDown(KEY_S))
    {
        transform2D.position += glm::vec2(0, -150 * dt);
    }
    if (engineContext.inputManager->IsKeyDown(KEY_D))
    {
        transform2D.position += glm::vec2(150 * dt, 0);
    }

    if (engineContext.inputManager->IsKeyDown(KEY_SPACE))
    {
	SNAKE_LOG("player shot the bullet");

	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * glm::pi<float>());

	float angle = angleDist(gen);
    engineContext.stateManager->GetCurrentState()->GetObjectManager().AddObject(std::make_unique<Bullet>(transform2D.position, glm::vec2(std::cos(angle), std::sin(angle))));
   
    }
}

void Player::Draw(const EngineContext& engineContext)
{
    material->SetUniform("u_Color", glm::vec4(1.0, 1.0, 1.0, 1.0));
    material->SetUniform("u_Model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(transform2D.position, 0.0f)), glm::vec3(50)));
    material->SetUniform("u_Projection", glm::ortho(
	-engineContext.windowManager->GetWidth() / 2.0f, engineContext.windowManager->GetWidth() / 2.0f,
	-engineContext.windowManager->GetHeight() / 2.0f, engineContext.windowManager->GetHeight() / 2.0f
    ));
}

void Player::Free(const EngineContext& engineContext)
{
    material->SetUniform("u_Model", glm::mat4(1.0f));
    SNAKE_LOG("Player Free Called");
}

void Player::LateFree(const EngineContext& engineContext)
{
    SNAKE_LOG("Player LateFree Called");
}
