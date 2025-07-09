#pragma once
#include "ObjectManager.h"
#include "Engine.h"

class Bullet : public GameObject
{
public:
    Bullet(glm::vec2 _pos);
    void Init() override;
    void Update(float dt, const EngineContext& engineContext) override;
    void Draw(const EngineContext& engineContext) override;
private:
    glm::vec2 pos;
    float timer = 0;
};

