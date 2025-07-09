#pragma once
#include"GameState.h"
#include "Player.h"

class MainMenu :public GameState
{
public:
    void Load() override;

    void Init() override;

    void Update(float dt, const EngineContext& engineContext) override;

    void Draw(const EngineContext& engineContext) override;

    void Free() override;

    void Unload() override;

private:
    Player* player;
};

