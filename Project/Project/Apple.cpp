#include "Apple.h"


Apple::Apple(Object* dependant_, int value_) : dependant(dependant_), value(value_)
{
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
    dependant->Kill();
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
