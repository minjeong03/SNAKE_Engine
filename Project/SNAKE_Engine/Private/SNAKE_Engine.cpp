#define GLAD_GL_IMPLEMENTATION
#include "glad/gl.h"
#include"glfw3/glfw3.h"


#include "SNAKE_Engine.h"
#include <iostream>
#include"glm/glm.hpp"

void SNAKE_engine::SetEngineContext(int windowWidth, int windowHeight)
{
    engineContext.stateManager = &stateManager;
    engineContext.windowManager = &windowManager;
    engineContext.inputManager = &inputManager;
    engineContext.engine = this;
}

SNAKE_engine::SNAKE_engine()
{

}

bool SNAKE_engine::Init(int windowWidth, int windowHeight)
{
    if (!windowManager.Init())
    {
        std::cerr << "Window Initialization failed." << std::endl;
        return false;
    }
    inputManager.Init(windowManager.GetHandle());
    SetEngineContext(windowWidth, windowHeight);

    return true;
}


void SNAKE_engine::Run()
{
    float last = static_cast<float>(glfwGetTime());

    while (shouldRun)
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
}

void SNAKE_engine::Shutdown() const
{
    glfwTerminate();
}

void SNAKE_engine::RequestQuit()
{
    shouldRun = false;
}
