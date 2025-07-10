#include "Level1.h"
#include <iostream>
#include "MainMenu.h"

#include "Engine.h"

void Level1::Load()
{
	std::cout << "[level1] load called\n";
	GameState::Load();
}

void Level1::Init()
{
	std::cout << "[level1] init called\n";

	objectManager.AddObject(std::make_unique<Player>());
}

void Level1::LateInit()
{
	GameState::LateInit();
}

void Level1::Update(float dt, const EngineContext& engineContext)
{
	if (engineContext.inputManager->IsKeyPressed(KEY_0))
	{
		std::cout << "[level1]0 pressed" << std::endl;
	}
	if (engineContext.inputManager->IsKeyReleased(KEY_0))
	{
		std::cout << "[level1]0 released" << std::endl;
	}
	if (engineContext.inputManager->IsKeyDown(KEY_0))
	{
		std::cout << "[level1]0 down" << std::endl;
	}
	if (engineContext.inputManager->IsKeyPressed(KEY_N))
	{
		std::cout << "[level1] key n pressed , move to mainmenu\n";
		engineContext.stateManager->ChangeState(std::make_unique<MainMenu>());
	}
	if (engineContext.inputManager->IsKeyPressed(KEY_ESCAPE))
	{
		engineContext.engine->RequestQuit();
	}
	if (engineContext.inputManager->IsKeyPressed(KEY_R))
	{
		Restart();
	}
}

void Level1::LateUpdate(float dt, const EngineContext& engineContext)
{
	GameState::LateUpdate(dt, engineContext);
}

void Level1::Draw(const EngineContext& engineContext)
{
	GameState::Draw(engineContext);
}

void Level1::Free()
{
	std::cout << "[level1] free called\n";
	GameState::Free();
}

void Level1::Unload()
{
	std::cout << "[level1] unload called\n";
	GameState::Unload();
}
