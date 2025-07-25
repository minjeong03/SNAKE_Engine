#include "Player.h"
#include <random>

#include "Bullet.h"
#include "Debug.h"
#include "Engine.h"


void Player::Init(const EngineContext& engineContext)
{
    transform2D.SetPosition(glm::vec2(0, 0));
    transform2D.SetScale(glm::vec2(50.f));

    SetMesh(engineContext, "default");
    SetMaterial(engineContext, "m_default");
    SetRenderLayer(engineContext, "Player");
}

void Player::LateInit(const EngineContext& engineContext)
{

}

void Player::Update(float dt, const EngineContext& engineContext)
{
    if (engineContext.inputManager->IsKeyDown(KEY_W))
    {
        transform2D.AddPosition(glm::vec2(0, 150 * dt));
    }
    if (engineContext.inputManager->IsKeyDown(KEY_A))
    {
        transform2D.AddPosition(glm::vec2(-150 * dt, 0));
    }
    if (engineContext.inputManager->IsKeyDown(KEY_S))
    {
        transform2D.AddPosition(glm::vec2(0, -150 * dt));
    }
    if (engineContext.inputManager->IsKeyDown(KEY_D))
    {
        transform2D.AddPosition(glm::vec2(150 * dt, 0));
    }

    if (engineContext.inputManager->IsKeyPressed(KEY_SPACE))
    {
        SNAKE_LOG("player shot the bullet");

        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * glm::pi<float>());

        float angle = angleDist(gen);
        std::unique_ptr<Bullet> b = std::make_unique<Bullet>(transform2D.GetPosition(), glm::vec2(std::cos(angle), std::sin(angle)));
        engineContext.stateManager->GetCurrentState()->GetObjectManager().AddObject(std::move(b),"bullet");
    }
}

void Player::Draw(const EngineContext& engineContext)
{
    //GetMaterial()->SetUniform("u_Color", glm::vec4(1.0, 1.0, 1.0, 1.0));
}

void Player::Free(const EngineContext& engineContext)
{
    SNAKE_LOG("Player Free Called");
}

void Player::LateFree(const EngineContext& engineContext)
{
    SNAKE_LOG("Player LateFree Called");
}
