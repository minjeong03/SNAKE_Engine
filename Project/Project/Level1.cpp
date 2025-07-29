#include "Level1.h"
#include <iostream>
#include "Debug.h"
#include "MainMenu.h"

#include "Engine.h"
#include "TextObject.h"
#include "Apple.h"

void Level1::Load(const EngineContext& engineContext)
{
    SNAKE_LOG("[Level1] load called");

    engineContext.renderManager->RegisterTexture("t_apple", "Textures/apple.png");
    engineContext.renderManager->RegisterTexture("t_background", "Textures/tiled_pattern_800x480.png");
    engineContext.renderManager->RegisterMaterial("m_apple", "s_default", { std::pair<std::string, std::string>("u_Texture","t_apple") });
    engineContext.renderManager->RegisterMaterial("m_background", "s_default", { std::pair<std::string, std::string>("u_Texture","t_background") });
}

void Level1::Init(const EngineContext& engineContext)
{
    SNAKE_LOG("[Level1] init called");

    auto font = engineContext.renderManager->GetFontByTag("default");

    cameraManager.GetActiveCamera()->SetPosition(
        { 
            engineContext.windowManager->GetWidth() * 0.5f - margin_x * multiplier,
            engineContext.windowManager->GetHeight() * 0.5f - margin_y * multiplier
        });
    cameraManager.GetActiveCamera()->SetZoom(1.0f);

    engineContext.soundManager->Play("bgm", 1, 20);

   
    for (int col = 0; col < cols; ++col)
    {
        glm::vec2 pos = { col * (spacing_x + apple_size_x) * multiplier, 0 };
        Apple* apple = (Apple*)objectManager.AddObject(std::make_unique<Apple>(), "apple");
        apple->GetTransform2D().SetPosition(pos);
        apple->GetTransform2D().SetScale({ apple_size_x, apple_size_y });
        apple->SetRenderLayer(engineContext, "Game");

        auto text = new TextObject(engineContext.renderManager->GetFontByTag("default"), std::to_string(apple->GetValue()), TextAlignH::Center, TextAlignV::Middle);
        objectManager.AddObject(std::unique_ptr<TextObject>(text), "apple_text");
        text->GetTransform2D().SetPosition(pos);
        text->SetRenderLayer(engineContext, "UI");
    }


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