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
public:
    SNAKE_engine();
    bool Init(int windowWidth, int windowHeight);
    void Run();
    void Shutdown();
    void RequestQuit();
    
    EngineContext& GetEngineContext() { return engineContext; }

private:
    EngineContext engineContext;
    StateManager stateManager;
    WindowManager windowManager;
    InputManager inputManager;

    bool shouldRun = true;
    void SetEngineContext(int windowWidth, int windowHeight);
};
