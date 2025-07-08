#include "Level1.h"

#include <iostream>

#include "EngineContext.h"
#include "MainMenu.h"
#include "SNAKE_Engine.h"


void Level1::Load()
{
	GameState::Load();
}

void Level1::Init()
{
	GameState::Init();
}
void Level1::Update(float dt, const EngineContext& engineContext)
{
	if (engineContext.inputManager->IsKeyPressed(KEY_0))
	{
		std::cout << "0 pressed" << std::endl;
	}
	if (engineContext.inputManager->IsKeyReleased(KEY_0))
	{
		std::cout << "0 released" << std::endl;
	}
	if (engineContext.inputManager->IsKeyDown(KEY_0))
	{
		std::cout << "0 down" << std::endl;
	}
	if (engineContext.inputManager->IsKeyDown(KEY_N))
	{
		engineContext.stateManager->ChangeState(std::make_unique<MainMenu>());
	}
	if (engineContext.inputManager->IsKeyDown(KEY_ESCAPE))
	{
		engineContext.engine->RequestQuit();
	}
}
void Level1::Draw()
{
	GameState::Draw();
}

void Level1::Free()
{
	GameState::Free();
}

void Level1::Unload()
{
	GameState::Unload();
}
