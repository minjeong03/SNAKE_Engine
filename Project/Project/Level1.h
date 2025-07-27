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
    void SetupCamera(const EngineContext& engineContext);
    void SetupPlayer(const EngineContext& engineContext);
    void SetupUI(const EngineContext& engineContext);
    void SetupMultilingualText(const EngineContext& engineContext);
    void SetupEnemy(const EngineContext& engineContext);
    void SetupMinimapCamera(const EngineContext& engineContext);

    void HandleStateInput(const EngineContext& engineContext);
    void HandleSoundInput(const EngineContext& engineContext);
    void HandleCameraInput(float dt, const EngineContext& engineContext);
    void HandleDebugDrawInput(const EngineContext& engineContext);
    void UpdateUIText(const EngineContext& engineContext);

    std::string GetVolumeText() const;

    Player* player = nullptr;
    Enemy* enemy = nullptr;

    TextObject* volumeDisplayText = nullptr;
    TextObject* bulletCountText = nullptr;

    float volume = 1.0f;
    bool trigger = false;
    SoundInstanceID id = 0;
};
