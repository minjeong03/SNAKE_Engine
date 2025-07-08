#define GLAD_GL_IMPLEMENTATION
#include "glad/gl.h"
#include"glfw3/glfw3.h"
#include"glm/glm.hpp"

#include "SNAKE_Engine.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SNAKE_engine* engine = static_cast<SNAKE_engine*>(glfwGetWindowUserPointer(window));
    if (engine)
    {
        engine->GetEngineContext().windowHeight = height;
        engine->GetEngineContext().windowWidth = width;
        //std::cout << "changed: " <<engine->GetEngineContext().windowHeight<< " "<<engine->GetEngineContext().windowWidth << std::endl;
    }

}

void SNAKE_engine::SetEngineContext(int windowWidth, int windowHeight)
{
    engineContext.stateManager = &stateManager;
    engineContext.window = window;
    engineContext.windowWidth = windowWidth;
    engineContext.windowHeight = windowHeight;
    engineContext.engine = this;
}

SNAKE_engine::SNAKE_engine():window(nullptr)
{

}

bool SNAKE_engine::Init(int windowWidth, int windowHeight)
{
    if (!glfwInit()) 
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, "GLFW + glad Window", nullptr, nullptr);
    if (!window) 
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) 
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    SetEngineContext(windowWidth, windowHeight);

    return true;
}


void SNAKE_engine::Run()
{
    while (shouldRun)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.1f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();

        stateManager.Update(1,engineContext);
        stateManager.Draw();
    }

}

void SNAKE_engine::Shutdown()
{
    glfwTerminate();
}

void SNAKE_engine::RequestQuit()
{
    shouldRun = false;
}
