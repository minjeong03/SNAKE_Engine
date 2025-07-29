#include "Level1.h"
#include <iostream>
#include "Debug.h"
#include "Enemy.h"
#include "MainMenu.h"

#include "Engine.h"
#include "TextObject.h"

void Level1::Load(const EngineContext& engineContext)
{
    SNAKE_LOG("[Level1] load called");
}

void Level1::Init(const EngineContext& engineContext)
{
    SNAKE_LOG("[Level1] init called");

    auto font = engineContext.renderManager->GetFontByTag("default");

    cameraManager.GetActiveCamera()->SetPosition({ 0, 0 });
    cameraManager.GetActiveCamera()->SetZoom(1.0f);

    engineContext.soundManager->Play("bgm", 1, 20);
}

void Level1::LateInit(const EngineContext& engineContext)
{
}

void Level1::Update(float dt, const EngineContext& engineContext)
{
    HandleStateInput(engineContext);
    HandleSoundInput(engineContext);
}

void Level1::HandleStateInput(const EngineContext& engineContext)
{
    if (engineContext.inputManager->IsKeyPressed(KEY_N))
    {
        SNAKE_LOG("[Level1] key n pressed , move to mainmenu");
        engineContext.stateManager->ChangeState(std::make_unique<MainMenu>());
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_ESCAPE))
    {
        engineContext.engine->RequestQuit();
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_R))
    {
        Restart(engineContext);
    }
}

void Level1::HandleSoundInput(const EngineContext& engineContext)
{
    auto& input = *engineContext.inputManager;

    if (input.IsKeyPressed(KEY_W) || input.IsKeyPressed(KEY_A) || input.IsKeyPressed(KEY_S) || input.IsKeyPressed(KEY_D))
    {
        engineContext.soundManager->Play("click", 1.0f);
    }
}

void Level1::LateUpdate(float dt, const EngineContext& engineContext)
{
}

void Level1::Draw(const EngineContext& engineContext)
{
   objectManager.DrawAll(engineContext, cameraManager.GetActiveCamera());
}

void Level1::Free(const EngineContext& engineContext)
{
    engineContext.soundManager->ControlAll(SoundManager::SoundControlType::Stop);
    SNAKE_LOG("[Level1] free called");
}

void Level1::Unload(const EngineContext& engineContext)
{
    SNAKE_LOG("[Level1] unload called");
}