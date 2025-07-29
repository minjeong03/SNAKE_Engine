#pragma once

#include "GameState.h"
#include "Player.h"
#include "TextObject.h"
#include "Enemy.h"


class Level1 : public GameState
{
public:
    void Load(const EngineContext& engineContext) override;
    void Init(const EngineContext& engineContext) override;
    void LateInit(const EngineContext& engineContext) override;
    void Update(float dt, const EngineContext& engineContext) override;
    void LateUpdate(float dt, const EngineContext& engineContext) override;
    void Draw(const EngineContext& engineContext) override;
    void Free(const EngineContext& engineContext) override;
    void Unload(const EngineContext& engineContext) override;

private:
    void HandleStateInput(const EngineContext& engineContext);
    void HandleSoundInput(const EngineContext& engineContext);
    
    float multiplier = 1.5f;
   
    const int rows = 10;
    const int cols = 17;

    const int margin_x = 45;
    const int margin_y = 40;

    const int spacing_x = 10;
    const int spacing_y = 8;

    const int apple_size_x = 32;
    const int apple_size_y = 35;

};
