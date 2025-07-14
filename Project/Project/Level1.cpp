#include "Level1.h"
#include <iostream>

#include "Debug.h"
#include "MainMenu.h"

#include "Engine.h"

void Level1::Load(const EngineContext& engineContext)
{
    SNAKE_LOG("[level1] load called");
    engineContext.renderManager->RegisterMesh("default", std::make_unique<Mesh>(std::vector<float>{
        -0.5f, -0.5f, 0.f, 0.f, 0.f, // vertex 0
            0.5f, -0.5f, 0.f, 1.f, 0.f, // vertex 1
            0.5f, 0.5f, 0.f, 1.f, 1.f, // vertex 2
            -0.5f, 0.5f, 0.f, 0.f, 1.f  // vertex 3
    }, std::vector<unsigned int>{0, 1, 2,
	2, 3, 0}));
    static std::vector<float> starVertices = {
        // pos         // uv (mapped to [0, 1])
         0.0f,  0.5f, 0.0f,   0.5f, 1.0f,  // top
         0.2f,  0.1f, 0.0f,   0.7f, 0.6f,
         0.5f,  0.1f, 0.0f,   1.0f, 0.6f,
         0.3f, -0.1f, 0.0f,   0.8f, 0.4f,
         0.4f, -0.5f, 0.0f,   0.9f, 0.0f,
         0.0f, -0.2f, 0.0f,   0.5f, 0.3f,
        -0.4f, -0.5f, 0.0f,   0.1f, 0.0f,
        -0.3f, -0.1f, 0.0f,   0.2f, 0.4f,
        -0.5f,  0.1f, 0.0f,   0.0f, 0.6f,
        -0.2f,  0.1f, 0.0f,   0.3f, 0.6f
    };

    static std::vector<unsigned int> starIndices = {
        0, 1, 9,    // top -> inner right -> inner left
        1, 2, 3,
        1, 3, 5,
        3, 4, 5,
        5, 6, 7,
        5, 7, 9,
        7, 8, 9,
        9, 0, 1
    };
	auto starMesh = std::make_unique<Mesh>(starVertices, starIndices);
    engineContext.renderManager->RegisterMesh("star", std::move(starMesh));

    TextureSettings ts;
    engineContext.renderManager->RegisterTexture("default", "Textures/Default.jpg", ts);
    engineContext.renderManager->RegisterTexture("uvchecker", "Textures/uvchecker.jpg", ts);
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
