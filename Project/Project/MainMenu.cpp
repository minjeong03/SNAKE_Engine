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
	if (engineContext.inputManager->IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		std::cout << "[mainmenu] left mouse button click" << std::endl;
	}
	if (engineContext.inputManager->IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		std::cout << "[mainmenu] left mouse button down" << std::endl;
	}
	if (engineContext.inputManager->IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		std::cout << "[mainmenu] left mouse button released" << std::endl;
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
