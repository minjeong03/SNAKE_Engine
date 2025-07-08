#include "MainMenu.h"

#include <iostream>

#include "Level1.h"
#include "Engine.h"

void MainMenu::Load()
{
	std::cout << "[mainmenu] load called\n";
	GameState::Load();
}

void MainMenu::Init()
{
	std::cout << "[mainmenu] init called\n";
	GameState::Init();
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

void MainMenu::Draw()
{
	GameState::Draw();
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
