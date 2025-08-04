#pragma once

#include "InputManager.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "StateManager.h"
#include "WindowManager.h"

struct GLFWwindow;
class SNAKE_Engine;

struct EngineContext
{
    StateManager* stateManager = nullptr;
    WindowManager* windowManager = nullptr;
    InputManager* inputManager = nullptr;
    RenderManager* renderManager = nullptr;
    SoundManager* soundManager = nullptr;
    SNAKE_Engine* engine = nullptr;
};
