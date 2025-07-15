#define GLAD_GL_IMPLEMENTATION
#include "glad/gl.h"
#include"glfw3/glfw3.h"
#ifdef _DEBUG
#include<vld.h>//TODO: remove this and directories before release (VC++ Directories -> Include Directories & Library Directories)
#endif
#include "SNAKE_Engine.h"


#include "Debug.h"
#include "EngineTimer.h"


void SNAKE_Engine::SetEngineContext()
{
    engineContext.stateManager = &stateManager;
    engineContext.windowManager = &windowManager;
    engineContext.inputManager = &inputManager;
    engineContext.renderManager = &renderManager;
    engineContext.engine = this;
}


bool SNAKE_Engine::Init(int windowWidth, int windowHeight)
{
    if (!windowManager.Init(windowWidth, windowHeight,*this))
    {
        SNAKE_ERR("Window Initialization failed.");
        return false;
    }
    inputManager.Init(windowManager.GetHandle());
    SetEngineContext();
    return true;
}


void SNAKE_Engine::Run()
{
    EngineTimer timer;
    timer.Start();

    while (shouldRun && !glfwWindowShouldClose(windowManager.GetHandle()))
    {
        float dt = timer.Tick();

        float fps = 0.0f;
        if (timer.ShouldUpdateFPS(fps))
        {
            windowManager.SetTitle("SNAKE_Engine - FPS: " + std::to_string(static_cast<int>(fps)));
        }

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
