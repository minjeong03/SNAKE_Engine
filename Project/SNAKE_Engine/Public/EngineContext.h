#pragma once

#include "InputManager.h"
#include "RenderManager.h"
#include "StateManager.h"
#include "WindowManager.h"

struct GLFWwindow;
class SNAKE_Engine;

/**
 * @brief Shared access point to all major engine systems.
 *
 * Acts as a container that holds pointers to engine-wide subsystems:
 * - StateManager
 * - WindowManager
 * - InputManager
 * - RenderManager
 * - Root engine instance (SNAKE_Engine)
 *
 * This struct is owned and initialized by the engine, and passed to game systems
 * like GameState, GameObject, and custom logic that need coordinated access
 * to rendering, input, or state transitions.
 *
 * Example usage:
 * @code
 * void Player::Update(float dt, const EngineContext& engineContext)
 * {
 *     if (engineContext.inputManager->IsKeyPressed(KEY_SPACE))
 *     {
 *         Jump();
 *     }
 * }
 * @endcode
 */
struct EngineContext
{
    StateManager* stateManager = nullptr;    ///< Manages active and queued game states.
    WindowManager* windowManager = nullptr;  ///< Manages window size, title, and event polling.
    InputManager* inputManager = nullptr;    ///< Tracks real-time keyboard and mouse input.
    RenderManager* renderManager = nullptr;  ///< Handles draw call submission and batching.
    SNAKE_Engine* engine = nullptr;          ///< Root engine instance. Owns all subsystems.
};
