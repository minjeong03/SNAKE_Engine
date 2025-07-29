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
};
