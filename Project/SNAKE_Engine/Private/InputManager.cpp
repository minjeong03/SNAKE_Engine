#include "InputManager.h"

#include <iostream>

#include "glfw3/glfw3.h"

void InputManager::Init(GLFWwindow* _window)
{
    window = _window;
}

void InputManager::Update()
{
    previousKeyState = currentKeyState;

    for (int key = 0; key <= GLFW_KEY_LAST; ++key) {
        currentKeyState.set(key, glfwGetKey(window, key) == GLFW_PRESS);
    }

    glfwGetCursorPos(window, &mouseX, &mouseY);
}

bool InputManager::IsKeyDown(int key) const
{
    return currentKeyState.test(key);
}

bool InputManager::IsKeyPressed(int key) const
{
    return currentKeyState.test(key) && !previousKeyState.test(key);
}

bool InputManager::IsKeyReleased(int key) const
{
    return !currentKeyState.test(key) && previousKeyState.test(key);
}

bool InputManager::IsMouseButtonDown(int button) const
{
    return false;
}

bool InputManager::IsMouseButtonPressed(int button) const
{
    return false;
}

bool InputManager::IsMouseButtonReleased(int button) const
{
    return false;
}

double InputManager::GetMouseX() const
{
    return mouseX;
}

double InputManager::GetMouseY() const
{
    return mouseY;
}
