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

    SetupCamera(engineContext);
    SetupPlayer(engineContext);
    SetupUI(engineContext);
    SetupMultilingualText(engineContext);
    SetupEnemy(engineContext);
    SetupMinimapCamera(engineContext);

    volume = 1.0f;
    id = engineContext.soundManager->Play("bgm", 1, 20);
}

void Level1::SetupCamera(const EngineContext&)
{
    cameraManager.GetActiveCamera()->SetPosition({ 0, 0 });
    cameraManager.GetActiveCamera()->SetZoom(1.0f);
}

void Level1::SetupPlayer(const EngineContext& engineContext)
{
    player = static_cast<Player*>(objectManager.AddObject(std::make_unique<Player>(), "level1 player"));
    player->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
}

void Level1::SetupUI(const EngineContext& engineContext)
{
    auto font = engineContext.renderManager->GetFontByTag("default");

    bulletCountText = static_cast<TextObject*>(objectManager.AddObject(
        std::make_unique<TextObject>(font, "test", TextAlignH::Center, TextAlignV::Middle), "text"));
    bulletCountText->GetTransform2D().SetPosition({ 100, 100 });
    bulletCountText->GetTransform2D().SetScale({ 1, 1 });
    bulletCountText->SetRenderLayer(engineContext, "UI");
    bulletCountText->SetIgnoreCamera(true, cameraManager.GetActiveCamera());


    volumeDisplayText = static_cast<TextObject*>(objectManager.AddObject(
        std::make_unique<TextObject>(font, GetVolumeText(), TextAlignH::Left, TextAlignV::Top), "ui"));
    volumeDisplayText->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
    volumeDisplayText->GetTransform2D().SetPosition({ -engineContext.windowManager->GetWidth() / 2, engineContext.windowManager->GetHeight() / 2 });
    volumeDisplayText->GetTransform2D().SetScale({ 0.5f, 0.5f });
    volumeDisplayText->SetRenderLayer(engineContext, "UI");
}

void Level1::SetupMultilingualText(const EngineContext& engineContext)
{
    objectManager.AddObject(std::make_unique<TextObject>(
        engineContext.renderManager->GetFontByTag("kr"),
        u8"한글도 Support합니다 ^_^\n줄바꿈도 물론 지원합니다",
        TextAlignH::Center, TextAlignV::Middle), "text")->SetColor({ 0, 0, 1, 1 });

    objectManager.AddObject(std::make_unique<TextObject>(
        engineContext.renderManager->GetFontByTag("kr"),
        u8"정렬도 지원합니다\n특수문자(!@#$%^&*)도 지원합니다",
        TextAlignH::Center, TextAlignV::Middle), "text")->GetTransform2D().SetPosition({ 0, 200 });

    objectManager.AddObject(std::make_unique<TextObject>(
        engineContext.renderManager->GetFontByTag("cn"),
        u8"也支持汉字",
        TextAlignH::Center, TextAlignV::Middle), "text")->GetTransform2D().SetPosition({ 0, -100 });

    objectManager.AddObject(std::make_unique<TextObject>(
        engineContext.renderManager->GetFontByTag("jp"),
        u8"漢字にも対応しています",
        TextAlignH::Center, TextAlignV::Middle), "text")->GetTransform2D().SetPosition({ 0, -200 });
}

void Level1::SetupEnemy(const EngineContext&)
{
    enemy = static_cast<Enemy*>(objectManager.AddObject(std::make_unique<Enemy>(glm::vec2(0, 0)), "enemy"));
}

void Level1::SetupMinimapCamera(const EngineContext& engineContext)
{
    auto minimapCam = std::make_unique<Camera2D>(engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight());
    minimapCam->SetZoom(0.2f);
    cameraManager.RegisterCamera("minimap", std::move(minimapCam));
}

void Level1::LateInit(const EngineContext& engineContext)
{
}

void Level1::Update(float dt, const EngineContext& engineContext)
{
    volumeDisplayText->GetTransform2D().SetPosition({ -engineContext.windowManager->GetWidth() / 2, engineContext.windowManager->GetHeight() / 2 });
    HandleStateInput(engineContext);
    HandleSoundInput(engineContext);
    HandleCameraInput(dt, engineContext);
    HandleDebugDrawInput(engineContext);
    UpdateUIText(engineContext);
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
        engineContext.soundManager->Play("click", volume);
    }

    if (input.IsKeyPressed(KEY_P))
    {
        trigger = !trigger;
        engineContext.soundManager->ControlByID(trigger ? SoundManager::SoundControlType::Pause : SoundManager::SoundControlType::Resume, id);
    }

    if (input.IsKeyPressed(KEY_1))
    {
        volume += 0.1f;
        engineContext.soundManager->SetVolumeByID(id, volume);
        volumeDisplayText->SetText(GetVolumeText());
    }

    if (input.IsKeyPressed(KEY_2))
    {
        volume = std::max(0.0f, volume - 0.1f);
        engineContext.soundManager->SetVolumeByID(id, volume);
        volumeDisplayText->SetText(GetVolumeText());
    }
}

void Level1::HandleCameraInput(float dt, const EngineContext& engineContext)
{
    auto cam = cameraManager.GetActiveCamera();
    auto& input = *engineContext.inputManager;

    if (input.IsKeyDown(KEY_I)) cam->AddPosition({ 0, 100 * dt });
    if (input.IsKeyDown(KEY_J)) cam->AddPosition({ -100 * dt, 0 });
    if (input.IsKeyDown(KEY_K)) cam->AddPosition({ 0, -100 * dt });
    if (input.IsKeyDown(KEY_L)) cam->AddPosition({ 100 * dt, 0 });
    if (input.IsKeyDown(KEY_U)) cam->SetZoom(cam->GetZoom() + 0.1f * dt);
    if (input.IsKeyDown(KEY_O)) cam->SetZoom(cam->GetZoom() - 0.1f * dt);
}

void Level1::HandleDebugDrawInput(const EngineContext& engineContext)
{
    if (engineContext.inputManager->IsKeyPressed(KEY_3))
    {
        engineContext.engine->RenderDebugDraws(true);
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_4))
    {
        engineContext.engine->RenderDebugDraws(false);
    }
}

void Level1::UpdateUIText(const EngineContext& engineContext)
{
    std::vector<Object*> bullets;
    objectManager.FindByTag("bullet", bullets);
    for (auto* bullet : bullets)
    {
        engineContext.renderManager->DrawDebugLine(
            bullet->GetTransform2D().GetPosition(),
            player->GetWorldPosition(),
            cameraManager.GetActiveCamera());
    }

    objectManager.FindByTag("enemyBullet", bullets);
    bulletCountText->SetText(std::to_string(bullets.size()));
    bulletCountText->GetTransform2D().SetPosition(player->GetTransform2D().GetPosition() + glm::vec2(0, 50));

    engineContext.renderManager->DrawDebugLine(
        enemy->GetTransform2D().GetPosition(),
        player->GetWorldPosition(),
        cameraManager.GetActiveCamera(),
        { 1, 0, 1, 1 }, 3.0f);
}

std::string Level1::GetVolumeText() const
{
    return "This is UI / Current Volume: " + std::to_string(volume) + "\nVolume Up: 1 Down: 2";
}

void Level1::LateUpdate(float dt, const EngineContext& engineContext)
{
}

void Level1::Draw(const EngineContext& engineContext)
{
   // objectManager.DrawAll(engineContext, cameraManager.GetActiveCamera());
    auto& rm = *engineContext.renderManager;

    rm.SetViewport(0, 0, engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight());
    cameraManager.SetActiveCamera("main");
    player->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
    bulletCountText->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
    volumeDisplayText->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
    //bulletCountText->GetTransform2D().SetPosition(player->GetWorldPosition() + glm::vec2(0, 50));
    objectManager.DrawAll(engineContext, cameraManager.GetActiveCamera());
    rm.FlushDrawCommands(engineContext);

    rm.ClearBackground(10, 10, 200, 200, glm::vec4(0.3, 0.3, 1, 0));
    rm.SetViewport(10, 10, 200, 200);

    cameraManager.SetActiveCamera("minimap");
    cameraManager.SetScreenSize("minimap", 200, 200);
    player->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
    bulletCountText->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
    volumeDisplayText->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
   // bulletCountText->GetTransform2D().SetPosition(player->GetWorldPosition() + glm::vec2(0, 50));
    objectManager.DrawAll(engineContext, cameraManager.GetActiveCamera());

    rm.FlushDrawCommands(engineContext);
    rm.SetViewport(0, 0, engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight());
    cameraManager.SetActiveCamera("main");
    player->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
    bulletCountText->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
    volumeDisplayText->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
   // bulletCountText->GetTransform2D().SetPosition(player->GetWorldPosition() + glm::vec2(0, 50));
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