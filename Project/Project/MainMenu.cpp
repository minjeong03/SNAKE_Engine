#include "MainMenu.h"
#include <iostream>
#include "../ThirdParty/glad/gl.h"

#include "Debug.h"
#include "Enemy.h"
#include "Level1.h"

#include "Engine.h"
#include "TextObject.h"
std::string tmp = "main";
uint64_t id = -1;
bool trigger = false;
float volume = 1;
void MainMenu::Load(const EngineContext& engineContext)
{
    SNAKE_LOG("[MainMenu] load called");

    engineContext.renderManager->RegisterFont("default","Fonts/font.ttf", 32);

}

void MainMenu::Init(const EngineContext& engineContext)
{
    SNAKE_LOG("[MainMenu] init called");
    objectManager.AddObject(std::make_unique<Player>(), "mainmenu player");

    test = static_cast<TextObject*>(objectManager.AddObject(std::make_unique<TextObject>(engineContext.renderManager->GetFontByTag("default"), "test"), "ui"));

    test->GetTransform2D().SetPosition({ 100,100 });
    test->GetTransform2D().SetScale({ 0.5,0.5 });

    test->SetRenderLayer(engineContext, "UI");

    objectManager.AddObject(std::make_unique<TextObject>(engineContext.renderManager->GetFontByTag("default"), "hello world!"), "ui")->GetTransform2D().SetPosition({-250,250});

    objectManager.AddObject(std::make_unique<Enemy>(glm::vec2(50,50)), "enemy");
    objectManager.AddObject(std::make_unique<Enemy>(glm::vec2(0, 0)), "enemy");

    auto minimapCam = std::make_unique<Camera2D>(engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight());
    minimapCam->SetZoom(5.05f);
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
    if (engineContext.inputManager->IsKeyPressed(KEY_1))
    {
        volume += 0.1f;
        engineContext.soundManager->SetVolumeByID(id, volume);
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_2))
    {
        volume -= 0.1f;
        engineContext.soundManager->SetVolumeByID(id, volume);
    }
    std::vector<Object*> list;
    objectManager.FindByTag("bullet", list);
    test->SetText(std::to_string(list.size()));

    test->GetTransform2D().SetPosition(objectManager.FindByTag("mainmenu player")->GetTransform2D().GetPosition()+glm::vec2(0,50));
}

void MainMenu::LateUpdate(float dt, const EngineContext& engineContext)
{
    GameState::LateUpdate(dt, engineContext);
}

void MainMenu::Draw(const EngineContext& engineContext)
{
    auto& rm = *engineContext.renderManager;

    //rm.SubmitText(text, "bullet");
    //rm.SetViewport(0, 0, engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight());
    cameraManager.SetActiveCamera("main");
    objectManager.DrawObjectsWithTag(engineContext, nullptr, "ui");
    objectManager.DrawObjectsWithTag(engineContext, cameraManager.GetActiveCamera(), "bullet");
    //rm.FlushDrawCommands(engineContext);
    objectManager.DrawObjectsWithTag(engineContext, cameraManager.GetActiveCamera(), "mainmenu player");
    //rm.ClearBackground(10, 10, 200, 200, glm::vec4(0.3, 0.3, 1, 0));
    //rm.SetViewport(10, 10, 200, 200);
    //rm.FlushDrawCommands(engineContext);
    objectManager.DrawObjectsWithTag(engineContext, cameraManager.GetActiveCamera(), "enemy");
    //cameraManager.SetActiveCamera("minimap");
	//cameraManager.SetScreenSize("minimap", 200, 200);


    //rm.FlushDrawCommands(engineContext);

    //rm.SetViewport(0, 0, engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight());
}


void MainMenu::Free(const EngineContext& engineContext)
{
    SNAKE_LOG("[MainMenu] free called");
}

void MainMenu::Unload(const EngineContext& engineContext)
{
    SNAKE_LOG("[MainMenu] unload called");
}
