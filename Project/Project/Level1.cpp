#include "Level1.h"
#include <iostream>

#include "Debug.h"
#include "MainMenu.h"

#include "Engine.h"

void Level1::Load(const EngineContext& engineContext)
{
    SNAKE_LOG("[level1] load called");
    engineContext.renderManager->RegisterMesh("default", std::make_unique<Mesh>(std::vector<float>{ -0.5f, -0.5f, 0.f, 0.5f, -0.5f, 0.f, 0.5f, 0.5f, 0.f, -0.5f, 0.5f, 0.f  }, std::vector<unsigned int>{0, 1, 2,
	2, 3, 0}));
    engineContext.renderManager->RegisterShader("default", { {ShaderStage::Vertex,"Shaders/Default.vert"},{ShaderStage::Fragment,"Shaders/Default.frag"} });
}

void Level1::Init(const EngineContext& engineContext)
{
    SNAKE_LOG("[level1] init called");
    objectManager.AddObject(std::make_unique<Player>(), "level1 player");
}

void Level1::LateInit(const EngineContext& engineContext)
{
}

void Level1::Update(float dt, const EngineContext& engineContext)
{
    if (engineContext.inputManager->IsKeyPressed(KEY_0))
    {
	SNAKE_LOG("[level1] 0 pressed");
    }
    if (engineContext.inputManager->IsKeyReleased(KEY_0))
    {
	SNAKE_LOG("[level1] 0 released");
    }
    if (engineContext.inputManager->IsKeyDown(KEY_0))
    {
	SNAKE_LOG("[level1] 0 down");
    }
    if (engineContext.inputManager->IsKeyPressed(KEY_N))
    {
	SNAKE_LOG("[level1] key n pressed , move to mainmenu");
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

void Level1::LateUpdate(float dt, const EngineContext& engineContext)
{
    GameState::LateUpdate(dt, engineContext);
}

void Level1::Draw(const EngineContext& engineContext)
{
    GameState::Draw(engineContext);
}

void Level1::Free(const EngineContext& engineContext)
{
    SNAKE_LOG("[level1] free called");
}

void Level1::Unload(const EngineContext& engineContext)
{
    SNAKE_LOG("[level1] unload called");
}
