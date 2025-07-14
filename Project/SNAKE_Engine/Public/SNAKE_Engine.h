#pragma once

#include "EngineContext.h"

/**
 * @brief Main class that initializes and runs the game engine.
 *
 * @details
 * SNAKE_Engine manages the initialization, execution, and shutdown of the core engine systems.
 * It handles:
 * - Creating the game window
 * - Managing input and frame timing
 * - Running the main game loop
 * - Delegating logic and rendering to StateManager
 *
 * EngineContext is initialized once and passed to other systems for access to shared resources.
 *
 * @author Jinwoo Choi
 * @date 2025-07-08
 */
class SNAKE_Engine
{
public:
    /** Constructs a new engine instance. */
    SNAKE_Engine();

    /**
     * @brief Initializes window, input system, and engine context.
     * @param windowWidth Width of the game window.
     * @param windowHeight Height of the game window.
     * @return True if all systems initialized successfully.
     *
     * @details
     * Internally initializes GLFW, creates the window, sets up input handling,
     * and binds subsystems to EngineContext.
     */
    [[nodiscard]]
    bool Init(int windowWidth, int windowHeight);

    /**
     * @brief Starts the engine's main loop.
     *
     * @details
     * Each frame performs the following steps:
     * - Poll window events
     * - Update input states
     * - Clear the screen
     * - Call StateManager's Update and Draw
     * - Swap buffers
     *
     * The loop continues until `RequestQuit()` is called or the window is closed.
     */
    void Run();


    /**
     * @brief Requests the engine to stop running.
     *
     * @details
     * This sets an internal flag so the Run() loop exits cleanly.
     */
    void RequestQuit();

    /**
     * @brief Returns a reference to the engine's context.
     * @return Reference to EngineContext containing pointers to core systems.
     */
    [[nodiscard]]
    EngineContext& GetEngineContext() { return engineContext; }

private:
    /**
     * @brief Internal helper to populate EngineContext with pointers to subsystems.
     * @param windowWidth Width of the game window (not used directly here).
     * @param windowHeight Height of the game window (not used directly here).
     */

     /**
      * @brief Cleans up engine systems and terminates GLFW.
      */
    void Free() const;

    void SetEngineContext(int windowWidth, int windowHeight);

    /** Central shared context passed to all systems (input, window, state, etc.). */
    EngineContext engineContext;

    /** Manages state transitions and delegates per-state Update/Draw calls. */
    StateManager stateManager;

    /** Handles window creation, OpenGL context, and buffer swapping. */
    WindowManager windowManager;

    /** Polls and tracks keyboard and mouse input states. */
    InputManager inputManager;

    RenderManager renderManager;
    /** Controls whether the main loop is running. */
    bool shouldRun = true;
};