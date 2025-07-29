#include "MainMenu.h"
#include <iostream>
#include "Button.h"
#include "Debug.h"
#include "Level1.h"

void MainMenu::Load(const EngineContext& engineContext)
{
    SNAKE_LOG("[MainMenu] load called");
}

void MainMenu::Init(const EngineContext& engineContext)
{
    SNAKE_LOG("[MainMenu] init called");

    playText = static_cast<TextObject*>(objectManager.AddObject(std::make_unique<TextObject>(engineContext.renderManager->GetFontByTag("default"),"START",TextAlignH::Center, TextAlignV::Middle), "playText"));
    playText->GetTransform2D().SetPosition({ 0,100 });
    playText->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
    playText->SetRenderLayer(engineContext, "UI");

    playButton = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<Button>(), "playButton"));
    playButton->GetTransform2D().SetPosition({ playText->GetWorldPosition() });
    playButton->GetTransform2D().SetScale({ playText->GetWorldScale()*1.5f });
}

void MainMenu::LateInit(const EngineContext& engineContext)
{
}

void MainMenu::Update(float dt, const EngineContext& engineContext)
{
    if (engineContext.inputManager->IsKeyPressed(KEY_N))
    {
        SNAKE_LOG("[MainMenu] key n pressed , move to level1");
        engineContext.stateManager->ChangeState(std::make_unique<Level1>());
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_ESCAPE))
    {
        engineContext.engine->RequestQuit();
    }

    if (engineContext.inputManager->IsKeyPressed(KEY_3))
    {
        engineContext.engine->RenderDebugDraws(true);
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_4))
    {
        engineContext.engine->RenderDebugDraws(false);
    }


    if (playButton->GetCollider()->CheckPointCollision( 
        cameraManager.GetActiveCamera()->GetPosition() + glm::vec2{ engineContext.inputManager->GetMouseX()-engineContext.windowManager->GetWidth()/2, engineContext.windowManager->GetHeight()/2-engineContext.inputManager->GetMouseY() }/cameraManager.GetActiveCamera()->GetZoom()))
    {
        playButton->SetColor({ 0.3,0.3,0.3,1.0 });
        playText->SetColor({ 0.3,0.3,0.3,1.0 });
        if (engineContext.inputManager->IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            engineContext.stateManager->ChangeState(std::make_unique<Level1>());
        }
    }
    else
    {
        playButton->SetColor({ 1.0,1.0,1.0,1.0});
        playText->SetColor({ 1.0,1.0,1.0,1.0 });
    }
}

void MainMenu::LateUpdate(float dt, const EngineContext& engineContext)
{
}

void MainMenu::Draw(const EngineContext& engineContext)
{
    engineContext.renderManager->ClearBackground(0, 0, engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight(), { 0.2,0.2,0.5,1 });
    objectManager.DrawAll(engineContext, cameraManager.GetActiveCamera());
}


void MainMenu::Free(const EngineContext& engineContext)
{
    SNAKE_LOG("[MainMenu] free called");
}

void MainMenu::Unload(const EngineContext& engineContext)
{
    SNAKE_LOG("[MainMenu] unload called");
}
