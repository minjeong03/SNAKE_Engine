#include "MainMenu.h"
#include "Level1.h"
#include "Engine.h"

void MainMenu::Load()
{
	GameState::Load();
}

void MainMenu::Init()
{
	GameState::Init();
}

void MainMenu::Update(float dt, const EngineContext& engineContext)
{
	if (engineContext.inputManager->IsKeyPressed(KEY_N))
	{
		engineContext.stateManager->ChangeState(std::make_unique<Level1>());
	}
	if (engineContext.inputManager->IsKeyDown(KEY_ESCAPE))
	{
		engineContext.engine->RequestQuit();
	}
}

void MainMenu::Draw()
{
	GameState::Draw();
}

void MainMenu::Free()
{
	GameState::Free();
}

void MainMenu::Unload()
{
	GameState::Unload();
}
