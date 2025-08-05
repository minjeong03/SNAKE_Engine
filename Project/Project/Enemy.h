#pragma once
#include "ObjectManager.h"
#include "Engine.h"

class Enemy : public GameObject
{
public:
    Enemy(glm::vec2 pos);
    void Init(const EngineContext& engineContext) override;
    void LateInit(const EngineContext& engineContext) override;
    void Update(float dt, const EngineContext& engineContext) override;
    void Draw(const EngineContext& engineContext) override;
    void Free(const EngineContext& engineContext) override;
    void LateFree(const EngineContext& engineContext) override;
    void OnCollision(Object* other) override;
    bool CheckIdle();
private:
    bool checkIdle = true;
};

