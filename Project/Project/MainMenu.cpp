#include "MainMenu.h"
#include <iostream>
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



}

void MainMenu::Init(const EngineContext& engineContext)
{
    SNAKE_LOG("[MainMenu] init called");

    cameraManager.GetActiveCamera()->SetPosition({ 0,0 });
    cameraManager.GetActiveCamera()->SetZoom(1.0f);

    player = static_cast<Player*>(objectManager.AddObject(std::make_unique<Player>(), "mainmenu player"));


    test = static_cast<TextObject*>(objectManager.AddObject(std::make_unique<TextObject>(engineContext.renderManager->GetFontByTag("default"), "test", TextAlignH::Center, TextAlignV::Middle), "text"));
    test->GetTransform2D().SetPosition({ 100,100 });
    test->GetTransform2D().SetScale({ 1,1 });
    test->SetRenderLayer(engineContext, "UI");


    volume = 1.0;
    uiText = static_cast<TextObject*>(objectManager.AddObject(std::make_unique<TextObject>(engineContext.renderManager->GetFontByTag("default"), "This is UI / Current Volume: "+std::to_string(volume)+"\nVolume Up: 1 Down: 2", TextAlignH::Left, TextAlignV::Top), "ui"));
    uiText->SetIgnoreCamera(true);
    uiText->GetTransform2D().SetPosition({ -engineContext.windowManager->GetWidth()/2, engineContext.windowManager->GetHeight()/2});
    uiText->GetTransform2D().SetScale({ 0.5f,0.5f});
    uiText->SetRenderLayer(engineContext, "UI");


    objectManager.AddObject(std::make_unique<TextObject>(engineContext.renderManager->GetFontByTag("kr"), u8"한글도 Support합니다 ^_^\n줄바꿈도 물론 지원합니다", TextAlignH::Center, TextAlignV::Middle), "text")->SetColor({0,0,1,1});
    objectManager.AddObject(std::make_unique<TextObject>(engineContext.renderManager->GetFontByTag("kr"), u8"정렬도 지원합니다\n특수문자(!@#$%^&*)도 지원합니다", TextAlignH::Center, TextAlignV::Middle), "text")->GetTransform2D().SetPosition({ 0,200 });
    objectManager.AddObject(std::make_unique<TextObject>(engineContext.renderManager->GetFontByTag("cn"), u8"也支持汉字", TextAlignH::Center, TextAlignV::Middle), "text")->GetTransform2D().SetPosition({ 0,-100 });
    objectManager.AddObject(std::make_unique<TextObject>(engineContext.renderManager->GetFontByTag("jp"), u8"漢字にも対応しています", TextAlignH::Center, TextAlignV::Middle), "text")->GetTransform2D().SetPosition({ 0,-200 });
   
    enemy = static_cast<Enemy*>(objectManager.AddObject(std::make_unique<Enemy>(glm::vec2(0, 0)), "enemy"));

    auto minimapCam = std::make_unique<Camera2D>(engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight());
    minimapCam->SetZoom(0.2f);
    cameraManager.RegisterCamera("minimap", std::move(minimapCam));

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
    if (engineContext.inputManager->IsKeyPressed(KEY_R))
    {
        Restart(engineContext);
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
        uiText->SetText("This is UI / Current Volume: " + std::to_string(volume) + "\nVolume Up: 1 Down: 2");
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_2))
    {
        if (volume >= 0.1f)
            volume -= 0.1f;
        else
            volume = 0;
        engineContext.soundManager->SetVolumeByID(id, volume);
        uiText->SetText("This is UI / Current Volume: " + std::to_string(volume) + "\nVolume Up: 1 Down: 2");
    }


    std::vector<Object*> bulletList;
    objectManager.FindByTag("bullet", bulletList);
    for (auto bullet : bulletList)
    {
        engineContext.renderManager->DrawDebugLine(bullet->GetTransform2D().GetPosition(), player->GetTransform2D().GetPosition(), cameraManager.GetActiveCamera());
    }
    objectManager.FindByTag("enemyBullet", bulletList);
    test->SetText(std::to_string(bulletList.size()));
    test->GetTransform2D().SetPosition(objectManager.FindByTag("mainmenu player")->GetTransform2D().GetPosition()+glm::vec2(0,50));

    engineContext.renderManager->DrawDebugLine(enemy->GetTransform2D().GetPosition(), player->GetTransform2D().GetPosition(), cameraManager.GetActiveCamera(),{ 1, 0, 1, 1 }, 3);




    if (engineContext.inputManager->IsKeyDown(KEY_I))
    {
        cameraManager.GetActiveCamera()->AddPosition({ 0,100 * dt });
    }
    if (engineContext.inputManager->IsKeyDown(KEY_J))
    {
        cameraManager.GetActiveCamera()->AddPosition({ -100 * dt,0 });
    }
    if (engineContext.inputManager->IsKeyDown(KEY_K))
    {
        cameraManager.GetActiveCamera()->AddPosition({ 0,-100 * dt });
    }
    if (engineContext.inputManager->IsKeyDown(KEY_L))
    {
        cameraManager.GetActiveCamera()->AddPosition({ 100 * dt,0 });
    }
    if (engineContext.inputManager->IsKeyDown(KEY_U))
    {
        cameraManager.GetActiveCamera()->SetZoom(cameraManager.GetActiveCamera()->GetZoom() + 0.1f *dt);
    }
    if (engineContext.inputManager->IsKeyDown(KEY_O))
    {
        cameraManager.GetActiveCamera()->SetZoom(cameraManager.GetActiveCamera()->GetZoom() - 0.1f * dt);
    }

    if (engineContext.inputManager->IsKeyPressed(KEY_3))
    {
        engineContext.engine->RenderDebugDraws(true);
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_4))
    {
        engineContext.engine->RenderDebugDraws(false);
    }
}

void MainMenu::LateUpdate(float dt, const EngineContext& engineContext)
{
    GameState::LateUpdate(dt, engineContext);
}

void MainMenu::Draw(const EngineContext& engineContext)
{
    //auto& rm = *engineContext.renderManager;

    //rm.SetViewport(0, 0, engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight());
    //cameraManager.SetActiveCamera("main");
    //objectManager.DrawObjectsWithTag(engineContext, nullptr, "ui");
    //objectManager.DrawObjectsWithTag(engineContext, cameraManager.GetActiveCamera(), "bullet");
    //objectManager.DrawObjectsWithTag(engineContext, cameraManager.GetActiveCamera(), "mainmenu player");
    //objectManager.DrawObjectsWithTag(engineContext, cameraManager.GetActiveCamera(), "enemy");
    //rm.FlushDrawCommands(engineContext);

    //rm.ClearBackground(10, 10, 200, 200, glm::vec4(0.3, 0.3, 1, 0));
    //rm.SetViewport(10, 10, 200, 200);

    //cameraManager.SetActiveCamera("minimap");
    //cameraManager.SetScreenSize("minimap", 200, 200);

    objectManager.DrawAll(engineContext, cameraManager.GetActiveCamera());

    //rm.FlushDrawCommands(engineContext);

    //rm.SetViewport(0, 0, engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight());
}


void MainMenu::Free(const EngineContext& engineContext)
{
    engineContext.soundManager->ControlAll(SoundManager::SoundControlType::Stop);
    SNAKE_LOG("[MainMenu] free called");
}

void MainMenu::Unload(const EngineContext& engineContext)
{
    SNAKE_LOG("[MainMenu] unload called");
}
