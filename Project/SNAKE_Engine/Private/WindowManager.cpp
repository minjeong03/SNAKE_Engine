#include "WindowManager.h"
#include <iostream>

#include "Debug.h"
#include "glad/gl.h"
#include "glfw3/glfw3.h"
#include "SNAKE_Engine.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    WindowManager* windowManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
    if (windowManager)
    {
        windowManager->SetWidth(width);
        windowManager->SetHeight(height);
        SNAKE_LOG("changed: " << windowManager->GetWidth() << " " << windowManager->GetHeight());
    }

}
bool WindowManager::Init()
{
    if (!glfwInit())
    {
        SNAKE_ERR("Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, "SNAKE ENGINE", nullptr, nullptr);
    if (!window)
    {
        SNAKE_ERR("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        SNAKE_ERR("Failed to initialize GLAD");
        return false;
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return true;
}

void WindowManager::Free()
{
    glfwDestroyWindow(window);
}

void WindowManager::SwapBuffers() const
{
    glfwSwapBuffers(window);
}

void WindowManager::ClearScreen() const
{
    glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void WindowManager::PollEvents() const
{
    glfwPollEvents();
}
