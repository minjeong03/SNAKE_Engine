#pragma once
#include "StateManager.h"
/**
 * @brief 
 * @details 
 * @author Jinwoo Choi
 * @date 2025-07-08
 */
struct GLFWwindow;

struct EngineContext
{
    StateManager* stateManager;
    GLFWwindow* window;
    unsigned int windowWidth;
    unsigned int windowHeight;
};
