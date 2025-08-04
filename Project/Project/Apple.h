#pragma once
#include "Engine.h"
#include "Timer.h"
#include "glm.hpp"

class Apple : public GameObject
{
public:
    Apple(Object* dependant, int value);
    void Init(const EngineContext& engineContext) override;
    void LateInit(const EngineContext& engineContext) override;
    void Update(float dt, const EngineContext& engineContext) override;
    void Draw(const EngineContext& engineContext) override;
    void Free(const EngineContext& engineContext) override;
    void LateFree(const EngineContext& engineContext) override;
    void OnCollision(Object* other) override;
    const int& GetValue() const;
    void SetSelected(bool selected);
    void SetVelocityAndStartDeadTimer(const glm::vec2& vel);
private:
    int value = 0;
    Object* dependant;
    const EngineContext* engineContext;
    glm::vec2 vel;    
    Timer dead_timer;
};

