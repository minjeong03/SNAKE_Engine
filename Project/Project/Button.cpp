#include "Button.h"

Button::Button()
{
}

void Button::Init(const EngineContext& engineContext)
{
    SetMesh(engineContext, "default");
    SetMaterial(engineContext, "m_blueMButton");
    SetCollider(std::make_unique<AABBCollider>(this, glm::vec2(0.5, 0.5)));
}

void Button::LateInit(const EngineContext& engineContext)
{
}

void Button::Update(float dt, const EngineContext& engineContext)
{
}

void Button::Draw(const EngineContext& engineContext)
{
}

void Button::Free(const EngineContext& engineContext)
{
}

void Button::LateFree(const EngineContext& engineContext)
{
}

void Button::OnCollision(Object* other)
{
    GameObject::OnCollision(other);
}
