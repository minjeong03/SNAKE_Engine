#include "InputManager.h"
#include "glfw3.h"

void InputManager::Init(GLFWwindow* _window)
{
    window = _window;
}

void InputManager::Update()
{
    previousKeyState = currentKeyState;
    previousMouseState = currentMouseState;

    for (int key = 0; key <= GLFW_KEY_LAST; key++)
    {
        currentKeyState.set(key, glfwGetKey(window, key) == GLFW_PRESS);
    }
    for (int button = 0; button <= GLFW_MOUSE_BUTTON_LAST; button++)
    {
        currentMouseState.set(button, glfwGetMouseButton(window, button) == GLFW_PRESS);
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
    return currentMouseState.test(button);
}

bool InputManager::IsMouseButtonPressed(int button) const
{
    return currentMouseState.test(button) && !previousMouseState.test(button);
}

bool InputManager::IsMouseButtonReleased(int button) const
{
    return !currentMouseState.test(button) && previousMouseState.test(button);
}

double InputManager::GetMouseX() const
{
    return mouseX;
}

double InputManager::GetMouseY() const
{
    return mouseY;
}

void InputManager::Clear()
{
    previousKeyState = currentKeyState;
    previousMouseState = currentMouseState;
    currentKeyState.reset();

    currentMouseState.reset();

}