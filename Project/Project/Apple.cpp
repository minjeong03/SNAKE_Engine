#include "Apple.h"
#include <random>

Apple::Apple()
{
    static std::random_device rd;                          // 하드웨어 기반 난수 (seed)
    static std::mt19937 gen(rd());                         // Mersenne Twister 19937 엔진
    static std::uniform_int_distribution<int> dist(1, 9);  // [1, 9] 범위의 균등 분포

    value = dist(gen);
}

void Apple::Init(const EngineContext& engineContext)
{
    SetMesh(engineContext, "default");
    SetMaterial(engineContext, "m_apple");
    SetCollider(std::make_unique<AABBCollider>(this, glm::vec2(0.4, 0.4))); 
    SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "apple", { "player_selection" });
}

void Apple::LateInit(const EngineContext& engineContext)
{
}

void Apple::Update(float dt, const EngineContext& engineContext)
{
}

void Apple::Draw(const EngineContext& engineContext)
{
}

void Apple::Free(const EngineContext& engineContext)
{
}

void Apple::LateFree(const EngineContext& engineContext)
{
}

void Apple::OnCollision(Object* other)
{
}

const int& Apple::GetValue() const
{
    return value;
}
