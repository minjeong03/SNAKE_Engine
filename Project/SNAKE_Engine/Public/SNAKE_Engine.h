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
    GLFWwindow* window;
    bool shouldRun = true;
    void SetEngineContext(int windowWidth, int windowHeight);
public:
    SNAKE_engine();
    bool Init(int windowWidth, int windowHeight);
    void Run();
    void Shutdown();
    void RequestQuit();
    
    EngineContext& GetEngineContext() { return engineContext; }

};
