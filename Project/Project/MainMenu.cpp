#include "MainMenu.h"
#include <iostream>

#include "Debug.h"
#include "Level1.h"

#include "Engine.h"

void MainMenu::Load(const EngineContext& engineContext)
{
	SNAKE_LOG("[MainMenu] load called");
}

void MainMenu::Init(const EngineContext& engineContext)
{
	SNAKE_LOG("[MainMenu] init called");
	objectManager.AddObject(std::make_unique<Player>());
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

}

void MainMenu::LateUpdate(float dt, const EngineContext& engineContext)
{
	GameState::LateUpdate(dt, engineContext);
}

void MainMenu::Draw(const EngineContext& engineContext)
{
	GameState::Draw(engineContext);
}

void MainMenu::Free(const EngineContext& engineContext)
{
	SNAKE_LOG("[MainMenu] free called");
}

void MainMenu::Unload(const EngineContext& engineContext)
{
	SNAKE_LOG("[MainMenu] unload called");
}
