#include "MainMenu.h"
#include <iostream>
#include "../ThirdParty/glad/gl.h"

#include "Debug.h"
#include "Level1.h"

#include "Engine.h"
std::string tmp = "main";
uint64_t id = -1;
bool trigger = false;
float volume = 1;
void MainMenu::Load(const EngineContext& engineContext)
{
    SNAKE_LOG("[MainMenu] load called");

}

void MainMenu::Init(const EngineContext& engineContext)
{
    SNAKE_LOG("[MainMenu] init called");
    objectManager.AddObject(std::make_unique<Player>(), "mainmenu player");

    auto minimapCam = std::make_unique<Camera2D>(engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight());
    minimapCam->SetZoom(0.05f);
    cameraManager.RegisterCamera("minimap", std::move(minimapCam));
    cameraManager.SetActiveCamera("minimap");

    id = engineContext.soundManager->Play("bgm", 1, 20);

}

void MainMenu::LateInit(const EngineContext& engineContext)
{
}

void MainMenu::Update(float dt, const EngineContext& engineContext)
{
    if (engineContext.inputManager->IsKeyPressed(KEY_N))
    {
        SNAKE_LOG("[MainMenu] key n pressed , move to mainmenu");
        engineContext.stateManager->ChangeState(std::make_unique<Level1>());
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_ESCAPE))
    {
        engineContext.engine->RequestQuit();
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_W))
    {
        engineContext.soundManager->Play("click", volume);
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_A))
    {
        engineContext.soundManager->Play("click", volume);
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_S))
    {
        engineContext.soundManager->Play("click", volume);
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_D))
    {
        engineContext.soundManager->Play("click", volume);
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_P))
    {
        trigger = !trigger;
        if (trigger)
            engineContext.soundManager->ControlByID(SoundManager::SoundControlType::Pause,id);
        else
            engineContext.soundManager->ControlByID(SoundManager::SoundControlType::Resume, id);
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_UP))
    {
        volume += 0.1f;
        engineContext.soundManager->SetVolumeByID(id, volume);
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_DOWN))
    {
        volume -= 0.1f;
        engineContext.soundManager->SetVolumeByID(id, volume);
    }
}

void MainMenu::LateUpdate(float dt, const EngineContext& engineContext)
{
    GameState::LateUpdate(dt, engineContext);
}

void MainMenu::Draw(const EngineContext& engineContext)
{
    auto& rm = *engineContext.renderManager;

    rm.SetViewport(0, 0, engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight());
    cameraManager.SetActiveCamera("main");
    objectManager.DrawAll(engineContext, cameraManager.GetActiveCamera());
    rm.FlushDrawCommands();

    rm.ClearBackground(10, 10, 200, 200, glm::vec4(0.3, 0.3, 1, 0));
    rm.SetViewport(10, 10, 200, 200);
    rm.FlushDrawCommands();

    cameraManager.SetActiveCamera("minimap");
    cameraManager.SetScreenSize("minimap", 200, 200);
    objectManager.DrawObjectsWithTag(engineContext, cameraManager.GetActiveCamera(), "bullet");
    rm.FlushDrawCommands();

    rm.SetViewport(0, 0, engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight());
    rm.FlushDrawCommands();
}


void MainMenu::Free(const EngineContext& engineContext)
{
    SNAKE_LOG("[MainMenu] free called");
}

void MainMenu::Unload(const EngineContext& engineContext)
{
    SNAKE_LOG("[MainMenu] unload called");
}
