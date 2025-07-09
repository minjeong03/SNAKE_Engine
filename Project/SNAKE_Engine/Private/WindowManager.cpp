#include "WindowManager.h"
#include <iostream>
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
        std::cout << "changed: " << windowManager ->GetWidth()<< " "<< windowManager->GetHeight()<< std::endl;
    }

}
bool WindowManager::Init()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, "SNAKE ENGINE", nullptr, nullptr);
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

    return true;
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
