#pragma once
#include "EngineContext.h"
/**
 * @brief 
 * @details 
 * @author Jinwoo Choi
 * @date 2025-07-08
 */
struct GLFWwindow;

class SNAKE_engine
{
private:
    EngineContext engineContext;
    StateManager stateManager;
    int windowWidth = 800;
    int windowHeight = 600;
    GLFWwindow* window;
public:
    bool Init(int argc, char* argv[]);
    bool Init();
    void Run();
    void Shutdown();

    StateManager& GetStateManager() { return stateManager; }
    int GetWidth() const { return windowWidth; }
    int GetHeight() const { return windowHeight; }
};
