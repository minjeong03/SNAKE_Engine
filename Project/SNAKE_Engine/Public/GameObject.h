#pragma once
#include "Object.h"


class GameObject : public Object
{
public:
    GameObject() : Object(ObjectType::GAME) {}
    ~GameObject() override = default;

    virtual void Init([[maybe_unused]] const EngineContext& engineContext) override {}

    virtual void LateInit([[maybe_unused]] const EngineContext& engineContext) override {}

    virtual void Update([[maybe_unused]] float dt, [[maybe_unused]] const EngineContext& engineContext) override {}

    virtual void Draw([[maybe_unused]] const EngineContext& engineContext) override {}

    virtual void Free([[maybe_unused]] const EngineContext& engineContext) override {}

    virtual void LateFree([[maybe_unused]] const EngineContext& engineContext) override {}
};
