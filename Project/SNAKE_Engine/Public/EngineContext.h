#pragma once
#include "InputManager.h"
#include "RenderManager.h"
#include "StateManager.h"
#include "WindowManager.h"

struct GLFWwindow;
class SNAKE_Engine;

/**
 * @brief Engine Context structure
 * @details
 * This struct provides unified access to core engine subsystems.
 * It is passed to GameObjects and systems to avoid global dependencies.
 * Includes access to managers such as state, window, input, and the engine itself.
 *
 * Use this to access shared engine-wide functionality during update, draw, or event handling.
 *
 * Example usage:
 *     engineContext.inputManager->IsKeyPressed(KEY_SPACE);
 *
 * @author Jinwoo Choi
 * @date 2025-07-08
 */
struct EngineContext
{
    StateManager* stateManager = nullptr;
    WindowManager* windowManager = nullptr;
    InputManager* inputManager = nullptr;
    RenderManager* renderManager = nullptr;
    SNAKE_Engine* engine = nullptr;
};
