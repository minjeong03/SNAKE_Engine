#pragma once
#include"GameState.h"
class MainMenu :public GameState
{
public:
    void Load() override;

    void Init() override;

    void Update(float dt, const EngineContext& engineContext) override;

    void Draw() override;

    void Free() override;

    void Unload() override;
};

