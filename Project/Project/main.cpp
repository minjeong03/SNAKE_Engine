#include <iostream>

#include "Debug.h"
#include "MainMenu.h"
#include "Engine.h"
#ifdef _DEBUG
//#include<vld.h>//TODO: remove this and directories before release (VC++ Directories -> Include Directories & Library Directories)
#endif
int main(int argc, char* argv[])
{
    SNAKE_Engine snakeEngine;
    DebugLogger::SetLogLevel(LogLevel::None);
    float multiplier = 1.5f;
    int width = 800* multiplier;
    int height = 480* multiplier;

    try
    {
        if (argc == 3)
        {
            width = std::stoi(argv[1]);
            height = std::stoi(argv[2]);
        }
        else if (argc != 1)
        {
            SNAKE_ERR("Usage: ./MyGame [width height]");
            return -1;
        }
    }
    catch (const std::exception& e)
    {
        SNAKE_ERR("Invalid arguments. Width and height must be integers.");
        return -1;
    }

    if (!snakeEngine.Init(width, height))
    {
        SNAKE_ERR("Engine initialization failed.");
        return -1;
    }
    snakeEngine.RenderDebugDraws(false);

    snakeEngine.GetEngineContext().renderManager->RegisterMesh("default", std::vector<Vertex>{
        {{-0.5f, -0.5f, 0.f}, { 0.f, 0.f }}, // vertex 0
        { { 0.5f, -0.5f, 0.f }, { 1.f, 0.f } }, // vertex 1
        { { 0.5f, 0.5f, 0.f }, { 1.f, 1.f } }, // vertex 2
        { { -0.5f, 0.5f, 0.f }, { 0.f, 1.f } }  // vertex 3
    }, std::vector<unsigned int>{0, 1, 2, 2, 3, 0});

    snakeEngine.GetEngineContext().renderManager->RegisterTexture("default", "Textures/Default.jpg");
    snakeEngine.GetEngineContext().renderManager->RegisterTexture("blueMButton", "Textures/blueMButton.png");
	snakeEngine.GetEngineContext().renderManager->RegisterTexture("penguinSpritesheet", "Textures/penguin.png");

    snakeEngine.GetEngineContext().renderManager->RegisterShader("s_default", { {ShaderStage::Vertex,"Shaders/Default.vert"},{ShaderStage::Fragment,"Shaders/Default.frag"} });
    snakeEngine.GetEngineContext().renderManager->RegisterShader("s_instancing", { {ShaderStage::Vertex,"Shaders/instancing.vert"},{ShaderStage::Fragment,"Shaders/instancing.frag"} });
    snakeEngine.GetEngineContext().renderManager->RegisterShader("s_animation", { {ShaderStage::Vertex,"Shaders/Animation.vert"},{ShaderStage::Fragment,"Shaders/Animation.frag"} });
    snakeEngine.GetEngineContext().renderManager->RegisterMaterial("m_animation", "s_animation", { });
    snakeEngine.GetEngineContext().renderManager->RegisterMaterial("m_instancing", "s_instancing", { std::pair<std::string, std::string>("u_Texture","default") });
    snakeEngine.GetEngineContext().renderManager->RegisterMaterial("m_blueMButton", "s_default", { std::pair<std::string, std::string>("u_Texture","blueMButton") });

    snakeEngine.GetEngineContext().renderManager->RegisterSpriteSheet("animTest", "penguinSpritesheet", 128, 128);

    snakeEngine.GetEngineContext().renderManager->RegisterRenderLayer("Game.Background");
    snakeEngine.GetEngineContext().renderManager->RegisterRenderLayer("Game");
    snakeEngine.GetEngineContext().renderManager->RegisterRenderLayer("UI");
    snakeEngine.GetEngineContext().renderManager->RegisterRenderLayer("UI.Pause");
    snakeEngine.GetEngineContext().renderManager->RegisterRenderLayer("UI.Pause.Text");
    snakeEngine.GetEngineContext().renderManager->RegisterRenderLayer("Bullet");
    snakeEngine.GetEngineContext().renderManager->RegisterRenderLayer("Penguin");
    snakeEngine.GetEngineContext().renderManager->RegisterRenderLayer("UI.Penguin");

    snakeEngine.GetEngineContext().soundManager->LoadSound("bgm", "Sounds/test.mp3");
    snakeEngine.GetEngineContext().soundManager->LoadSound("click", "Sounds/mouse.mp3");
    snakeEngine.GetEngineContext().soundManager->LoadSound("click1", "Sounds/mouse1.mp3");
    snakeEngine.GetEngineContext().soundManager->LoadSound("click2", "Sounds/mouse2.mp3");
    snakeEngine.GetEngineContext().soundManager->LoadSound("beep", "Sounds/beep.mp3");

    snakeEngine.GetEngineContext().renderManager->RegisterFont("default", "Fonts/NotoSans-VariableFont_wdth,wght.ttf", 50);
    snakeEngine.GetEngineContext().renderManager->RegisterFont("kr", "Fonts/NotoSansKR-VariableFont_wght.ttf", 50);

    snakeEngine.GetEngineContext().stateManager->ChangeState(std::make_unique<MainMenu>());

    snakeEngine.Run();

    return 0;
}