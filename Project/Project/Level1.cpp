#include "Level1.h"

#include <iostream>


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
	std::cout << "level1" << std::endl;
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
