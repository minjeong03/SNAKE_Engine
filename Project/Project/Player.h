#pragma once
#include "ObjectManager.h"
#include "Engine.h"

class Player : public GameObject
{
public:
    void Init() override;
    void Update(float dt, const EngineContext& engineContext) override;
    void Draw(const EngineContext& engineContext) override;
private:
    glm::vec2 pos;
};

