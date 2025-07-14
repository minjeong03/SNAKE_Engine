#define GLAD_GL_IMPLEMENTATION
#include "glad/gl.h"
#include"glfw3/glfw3.h"
#ifdef _DEBUG
#include<vld.h>//TODO: remove this and directories before release (VC++ Directories -> Include Directories & Library Directories)
#endif
#include "SNAKE_Engine.h"


#include "Debug.h"


void SNAKE_Engine::SetEngineContext(int windowWidth, int windowHeight)
{
    engineContext.stateManager = &stateManager;
    engineContext.windowManager = &windowManager;
    engineContext.inputManager = &inputManager;
    engineContext.renderManager = &renderManager;
    engineContext.engine = this;
}

SNAKE_Engine::SNAKE_Engine()
{

}

bool SNAKE_Engine::Init(int windowWidth, int windowHeight)
{
    if (!windowManager.Init())
    {
        SNAKE_ERR("Window Initialization failed.");
        return false;
    }
    inputManager.Init(windowManager.GetHandle());
    SetEngineContext(windowWidth, windowHeight);

    return true;
}


void SNAKE_Engine::Run()
{
    float last = static_cast<float>(glfwGetTime());
    while (shouldRun && !glfwWindowShouldClose(windowManager.GetHandle()))
    {
        float now = static_cast<float>(glfwGetTime());
        float dt = now - last;
        last = now;
        windowManager.PollEvents();
        inputManager.Update();

        windowManager.ClearScreen();

        stateManager.Update(dt, engineContext);
        stateManager.Draw(engineContext);

        windowManager.SwapBuffers();
    }
    stateManager.Free(engineContext);
    windowManager.Free();
    Free();
}

void SNAKE_Engine::Free() const
{
    glfwTerminate();
}

void SNAKE_Engine::RequestQuit()
{
    shouldRun = false;
}
