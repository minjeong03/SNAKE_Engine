#pragma once
#include "InputManager.h"
#include "StateManager.h"
#include "WindowManager.h"

struct GLFWwindow;
class SNAKE_engine;

/**
 * @brief
 * @details
 * @author Jinwoo Choi
 * @date 2025-07-08
 */
struct EngineContext
{
    StateManager* stateManager = nullptr;
    WindowManager* windowManager = nullptr;
    InputManager* inputManager = nullptr;
    SNAKE_engine* engine = nullptr;
};
