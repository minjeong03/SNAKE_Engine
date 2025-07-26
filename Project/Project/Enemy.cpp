#include "Enemy.h"

#include <random>

#include "Bullet.h"
#include "Debug.h"
#include "Engine.h"

Enemy::Enemy(glm::vec2 pos)
{
    transform2D.SetPosition(pos);
    transform2D.SetScale(glm::vec2(50.f));
}

void Enemy::Init(const EngineContext& engineContext)
{
    SetMesh(engineContext, "default");
    SetMaterial(engineContext, "m_default");
    SetRenderLayer(engineContext, "Enemy");
    SetColor({ 0.5,0.3,0.1,1 });
}

void Enemy::LateInit(const EngineContext& engineContext)
{

}

void Enemy::Update(float dt, const EngineContext& engineContext)
{
    if (engineContext.inputManager->IsKeyDown(KEY_UP))
    {
        transform2D.AddPosition(glm::vec2(0, 150 * dt));
    }
    if (engineContext.inputManager->IsKeyDown(KEY_LEFT))
    {
        transform2D.AddPosition(glm::vec2(-150 * dt, 0));
    }
    if (engineContext.inputManager->IsKeyDown(KEY_DOWN))
    {
        transform2D.AddPosition(glm::vec2(0, -150 * dt));
    }
    if (engineContext.inputManager->IsKeyDown(KEY_RIGHT))
    {
        transform2D.AddPosition(glm::vec2(150 * dt, 0));
    }

    if (engineContext.inputManager->IsKeyDown(KEY_RIGHT_CONTROL))
    {
        SNAKE_LOG("enemy shot the bullet");

        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * glm::pi<float>());

        for (int i = 0; i < 20; i++)
        {
            float angle = angleDist(gen);
            std::unique_ptr<Bullet> b = std::make_unique<Bullet>(transform2D.GetPosition(), glm::vec2(std::cos(angle), std::sin(angle)));
            engineContext.stateManager->GetCurrentState()->GetObjectManager().AddObject(std::move(b), "enemyBullet");
        }
    }
}

void Enemy::Draw(const EngineContext& engineContext)
{
    /*GetMaterial()->SetUniform("u_Color", glm::vec4(1.0, 0.0, 1.0, 1.0));
    GetMaterial()->SetUniform("u_Model", transform2D.GetMatrix());*/
}

void Enemy::Free(const EngineContext& engineContext)
{
    SNAKE_LOG("Player Free Called");
}

void Enemy::LateFree(const EngineContext& engineContext)
{
    SNAKE_LOG("Player LateFree Called");
}
