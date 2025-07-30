#include "Level1.h"
#include <iostream>
#include "Debug.h"
#include "MainMenu.h"
#include <random>

#include "Engine.h"
#include "TextObject.h"
#include "Apple.h"
#include "ApplePlayerController.h"

void Level1::Load(const EngineContext& engineContext)
{
    SNAKE_LOG("[Level1] load called");

    engineContext.renderManager->RegisterTexture("t_apple", "Textures/apple.png");
    engineContext.renderManager->RegisterTexture("t_apple_selected", "Textures/apple_highlighted.png");
    engineContext.renderManager->RegisterTexture("t_background", "Textures/tiled_pattern_800x480.png");
    engineContext.renderManager->RegisterTexture("t_selection_box", "Textures/TransparentSquare.png");
    engineContext.renderManager->RegisterTexture("t_border", "Textures/SquareBorder.png");
    engineContext.renderManager->RegisterTexture("t_fill", "Textures/Square.png");
    engineContext.renderManager->RegisterMaterial("m_apple", "s_default", { std::pair<std::string, std::string>("u_Texture","t_apple") });
    engineContext.renderManager->RegisterMaterial("m_apple_highlighted", "s_default", { std::pair<std::string, std::string>("u_Texture","t_apple_selected") });
    engineContext.renderManager->RegisterMaterial("m_background", "s_default", { std::pair<std::string, std::string>("u_Texture","t_background") });
    engineContext.renderManager->RegisterMaterial("m_selection_box", "s_default", { std::pair<std::string, std::string>("u_Texture","t_selection_box") });
    engineContext.renderManager->RegisterMaterial("m_border", "s_default", { std::pair<std::string, std::string>("u_Texture","t_border") });
    engineContext.renderManager->RegisterMaterial("m_fill", "s_default", { std::pair<std::string, std::string>("u_Texture","t_fill") });

    engineContext.engine->RenderDebugDraws(false);
}

void Level1::Init(const EngineContext& engineContext)
{
    static std::random_device rd;                          // 하드웨어 기반 난수 (seed)
    static std::mt19937 gen(rd());                         // Mersenne Twister 19937 엔진
    static std::uniform_int_distribution<int> dist(1, 9);  // [1, 9] 범위의 균등 분포

    SNAKE_LOG("[Level1] init called");

    auto font = engineContext.renderManager->GetFontByTag("default");

    cameraManager.GetActiveCamera()->SetPosition(
        {
            engineContext.windowManager->GetWidth() * 0.5f - margin_x * multiplier,
            engineContext.windowManager->GetHeight() * 0.5f - margin_y * multiplier
        });
    cameraManager.GetActiveCamera()->SetZoom(1.0f);

    engineContext.soundManager->Play("bgm", 1, 20);

    auto* background_obj = objectManager.AddObject(std::make_unique<GameObject>(), "background");
    background_obj->SetMesh(engineContext, "default");
    background_obj->SetMaterial(engineContext, "m_background");
    background_obj->GetTransform2D().SetScale({ engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight() });
    background_obj->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
    background_obj->SetRenderLayer(engineContext, "Game.Background");

    auto* score_ui_obj = objectManager.AddObject(std::make_unique<GameObject>(), "score_ui");
    score_ui_obj->SetMesh(engineContext, "default");
    score_ui_obj->SetMaterial(engineContext, "m_apple");
    score_ui_obj->GetTransform2D().SetPosition({ 0, 100 });
    score_ui_obj->GetTransform2D().SetScale({400, 400});
    score_ui_obj->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
    score_ui_obj->SetRenderLayer(engineContext, "UI.Pause");
    score_ui_obj->SetVisibility(false);

    score_ui_text = new TextObject(engineContext.renderManager->GetFontByTag("default"), std::to_string(0), TextAlignH::Center, TextAlignV::Middle);
    objectManager.AddObject(std::unique_ptr<TextObject>(score_ui_text), "score_ui_text");
    score_ui_text->GetTransform2D().SetPosition(score_ui_obj->GetTransform2D().GetPosition());
    score_ui_text->GetTransform2D().SetScale({ 10, 10 });
    score_ui_text->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
    score_ui_text->SetRenderLayer(engineContext, "UI.Pause.Text");
    score_ui_text->SetVisibility(false);

    objectManager.AddObject(std::make_unique<ApplePlayerController>(), "player_controller");

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            glm::vec2 pos = { col * (spacing_x + apple_size_x) * multiplier, row * (spacing_y + apple_size_y) * multiplier };
            int value = dist(gen); 

            auto text = new TextObject(engineContext.renderManager->GetFontByTag("default"), std::to_string(value), TextAlignH::Center, TextAlignV::Middle);
            objectManager.AddObject(std::unique_ptr<TextObject>(text), "apple_text");
            text->GetTransform2D().SetPosition(pos);
            text->SetRenderLayer(engineContext, "UI");

            Apple* apple = (Apple*)objectManager.AddObject(std::make_unique<Apple>(text, value), "apple");
            apple->GetTransform2D().SetPosition(pos);
            apple->GetTransform2D().SetScale({ apple_size_x, apple_size_y });
            apple->SetRenderLayer(engineContext, "Game");
        }
    }

    fill_initial_scale_y = engineContext.windowManager->GetHeight() * 0.8f;
    glm::vec2 pos = { cols * (spacing_x + apple_size_x) * multiplier, fill_initial_scale_y * 0.5f};

    auto timer_bar_background = objectManager.AddObject(std::make_unique<GameObject>(), "timer_bar_background");
    timer_bar_background->SetMesh(engineContext, "default");
    timer_bar_background->SetMaterial(engineContext, "m_border");
    timer_bar_background->SetColor({0.0f, 1.0f, 0.12f, 1.0f});
    timer_bar_background->GetTransform2D().SetPosition(pos);
    timer_bar_background->GetTransform2D().SetScale({ apple_size_x, fill_initial_scale_y });
    timer_bar_background->SetRenderLayer(engineContext, "Game");

    timer_bar_fill = objectManager.AddObject(std::make_unique<GameObject>(), "timer_bar_fill");
    timer_bar_fill->SetMesh(engineContext, "default");
    timer_bar_fill->SetMaterial(engineContext, "m_fill");
    timer_bar_fill->SetColor({ 0.0f, 1.0f, 0.12f, 1.0f });
    timer_bar_fill->GetTransform2D().SetPosition(pos);
    timer_bar_fill->GetTransform2D().SetScale({ apple_size_x, fill_initial_scale_y });
    timer_bar_fill->SetRenderLayer(engineContext, "Game");
    
    game_timer.Start(10);
}

void Level1::LateInit(const EngineContext& engineContext)
{
}

void Level1::Update(float dt, const EngineContext& engineContext)
{
    if (game_timer.IsTimedOut())
    {
        ApplePlayerController* player = (ApplePlayerController*)objectManager.FindByTag("player_controller");
        score_ui_text->SetText(std::to_string(player->GetScore()));
        score_ui_text->SetVisibility(true);
        objectManager.FindByTag("score_ui")->SetVisibility(true);
    }
    else
    {
        float current_scale_y = fill_initial_scale_y * (1 - game_timer.GetProgressPercentage());
        timer_bar_fill->GetTransform2D().SetScale(
            { timer_bar_fill->GetTransform2D().GetScale().x, current_scale_y });
        timer_bar_fill->GetTransform2D().SetPosition(
            { timer_bar_fill->GetTransform2D().GetPosition().x, current_scale_y * 0.5f});
        game_timer.Update(dt);
        HandleStateInput(engineContext);
        HandleSoundInput(engineContext);
        GameState::Update(dt, engineContext);
    }
}

void Level1::HandleStateInput(const EngineContext& engineContext)
{
    if (engineContext.inputManager->IsKeyReleased(KEY_N))
    {
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

    if (input.IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        engineContext.soundManager->Play("click1", 1.0f);
    }
    if (input.IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        engineContext.soundManager->Play("click2", 1.0f);
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

void Timer::Start(float time)
{
    this->time = time;
    elapsed = 0;
}

void Timer::Update(float dt)
{
    elapsed += dt;
}

bool Timer::IsTimedOut() const
{
    return elapsed >= time;
}

float Timer::GetProgressPercentage() const
{
    return elapsed / time;
}
