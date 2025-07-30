#include "Apple.h"


Apple::Apple(Object* dependant_, int value_) : dependant(dependant_), value(value_)
{
}

void Apple::Init(const EngineContext& engineContext)
{
    this->engineContext = &engineContext;
    SetMesh(engineContext, "default");
    SetSelected(false);
    SetCollider(std::make_unique<AABBCollider>(this, glm::vec2(0.9, 0.9))); 
    SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "apple", { "player_selection" });
}

void Apple::LateInit(const EngineContext& engineContext)
{
}

void Apple::Update(float dt, const EngineContext& engineContext)
{
    SetSelected(false);
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
    if (other->GetTag() == "player_controller")
    {
        SetSelected(true);
    }
}

const int& Apple::GetValue() const
{
    return value;
}

void Apple::SetSelected(bool bSelected)
{
    if (bSelected)
    {
        SetMaterial(*engineContext, "m_apple_highlighted");
    }
    else
    {
        SetMaterial(*engineContext, "m_apple");
    }
}