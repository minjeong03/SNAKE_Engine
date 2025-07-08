#pragma once
#include "StateManager.h"

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
    GLFWwindow* window = nullptr;
    SNAKE_engine* engine = nullptr;
    int windowWidth = 800;
    int windowHeight = 600;
};
