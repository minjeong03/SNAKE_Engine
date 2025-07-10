#include "MainMenu.h"
#include <iostream>
#include "Level1.h"

#include "Engine.h"

void MainMenu::Load()
{
	std::cout << "[mainmenu] load called\n";
}

void MainMenu::Init()
{
	std::cout << "[mainmenu] init called\n";
	objectManager.AddObject(std::make_unique<Player>());
}

void MainMenu::LateInit()
{
	GameState::LateInit();
}

void MainMenu::Update(float dt, const EngineContext& engineContext)
{
	if (engineContext.inputManager->IsKeyPressed(KEY_N))
	{
		std::cout << "[mainmenu] key n pressed , move to level1\n";
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

void MainMenu::Free()
{
	std::cout << "[mainmenu] free called\n";
	GameState::Free();
}

void MainMenu::Unload()
{
	std::cout << "[mainmenu] unload called\n";
	GameState::Unload();
}
