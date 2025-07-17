#include "Level1.h"
#include <iostream>

#include "Debug.h"
#include "MainMenu.h"

#include "Engine.h"

void Level1::Load(const EngineContext& engineContext)
{
    SNAKE_LOG("[level1] load called");
   
}

void Level1::Init(const EngineContext& engineContext)
{
    SNAKE_LOG("[level1] init called");
    objectManager.AddObject(std::make_unique<Player>(), "level1 player");

    auto mainCam = std::make_unique<Camera2D>(engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight());
    mainCam->SetZoom(1.0f);
    cameraManager.RegisterCamera("main", std::move(mainCam));

    auto minimapCam = std::make_unique<Camera2D>();
    minimapCam->SetZoom(0.1f);
    minimapCam->SetPosition({ 200, 200 });
    cameraManager.RegisterCamera("minimap", std::move(minimapCam));

    cameraManager.SetActiveCamera("main");
}

void Level1::LateInit(const EngineContext& engineContext)
{
}

void Level1::Update(float dt, const EngineContext& engineContext)
{
    if (engineContext.inputManager->IsKeyPressed(KEY_0))
    {
        SNAKE_LOG("[level1] 0 pressed");
    }
    if (engineContext.inputManager->IsKeyReleased(KEY_0))
    {
        SNAKE_LOG("[level1] 0 released");
    }
    if (engineContext.inputManager->IsKeyDown(KEY_0))
    {
        SNAKE_LOG("[level1] 0 down");
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_N))
    {
        SNAKE_LOG("[level1] key n pressed , move to mainmenu");
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

void Level1::LateUpdate(float dt, const EngineContext& engineContext)
{
    GameState::LateUpdate(dt, engineContext);
}

void Level1::Draw(const EngineContext& engineContext)
{
    objectManager.DrawAll(engineContext,cameraManager.GetActiveCamera());
}

void Level1::Free(const EngineContext& engineContext)
{
    SNAKE_LOG("[level1] free called");
}

void Level1::Unload(const EngineContext& engineContext)
{
    SNAKE_LOG("[level1] unload called");
}
