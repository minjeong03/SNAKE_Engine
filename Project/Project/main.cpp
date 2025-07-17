#include <iostream>

#include "Debug.h"
#include "MainMenu.h"
#include "Engine.h"
#ifdef _DEBUG
#include<vld.h>//TODO: remove this and directories before release (VC++ Directories -> Include Directories & Library Directories)
#endif
int main(int argc, char* argv[])
{
    SNAKE_Engine snakeEngine;

    int width = 800;
    int height = 600;

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
    snakeEngine.GetEngineContext().renderManager->RegisterMesh("default", std::vector<float>{
        -0.5f, -0.5f, 0.f, 0.f, 0.f, // vertex 0
            0.5f, -0.5f, 0.f, 1.f, 0.f, // vertex 1
            0.5f, 0.5f, 0.f, 1.f, 1.f, // vertex 2
            -0.5f, 0.5f, 0.f, 0.f, 1.f  // vertex 3
    }, std::vector<unsigned int>{0, 1, 2,
        2, 3, 0});
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
    snakeEngine.GetEngineContext().renderManager->RegisterMesh("star", starVertices, starIndices);
    TextureSettings ts;
    snakeEngine.GetEngineContext().renderManager->RegisterTexture("default", "Textures/Default.jpg", ts);
    snakeEngine.GetEngineContext().renderManager->RegisterTexture("uvchecker", "Textures/uvchecker.jpg", ts);
    snakeEngine.GetEngineContext().renderManager->RegisterShader("s_default", { {ShaderStage::Vertex,"Shaders/Default.vert"},{ShaderStage::Fragment,"Shaders/Default.frag"} });
    snakeEngine.GetEngineContext().renderManager->RegisterShader("s_instancing", { {ShaderStage::Vertex,"Shaders/Instancing.vert"},{ShaderStage::Fragment,"Shaders/Instancing.frag"} });
    snakeEngine.GetEngineContext().renderManager->RegisterMaterial("m_default", "s_default", { std::pair<std::string, std::string>("u_Texture","uvchecker") });
    snakeEngine.GetEngineContext().renderManager->RegisterMaterial("m_instancing", "s_instancing", { std::pair<std::string, std::string>("u_Texture","default") });
    snakeEngine.GetEngineContext().renderManager->RegisterRenderLayer("Bullet");
    snakeEngine.GetEngineContext().renderManager->RegisterRenderLayer("Player");
    snakeEngine.GetEngineContext().renderManager->RegisterRenderLayer("UI");

    snakeEngine.GetEngineContext().stateManager->ChangeState(std::make_unique<MainMenu>());

    snakeEngine.Run();

    return 0;
}