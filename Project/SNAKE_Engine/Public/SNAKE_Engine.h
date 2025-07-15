#pragma once

#include "EngineContext.h"

/**
 * @brief Main entry point and system coordinator for the entire engine lifecycle.
 *
 * @details
 * SNAKE_Engine initializes all subsystems, maintains the main game loop, and provides
 * access to the centralized EngineContext. This class must be created and executed by the user.
 *
 * @code
 * SNAKE_Engine engine;
 * if (engine.Init(1280, 720))
 * {
 *     engine.Run();
 * }
 * @endcode
 */
class SNAKE_Engine
{
public:
    /**
     * @brief Constructs the engine and allocates all core systems.
     *
     * @details
     * This constructor only allocates internal members. You must call Init() before Run().
     */
    SNAKE_Engine() = default;

    /**
     * @brief Initializes window, input, rendering, and all engine subsystems.
     *
     * @details
     * Sets up OpenGL context and links all systems into the EngineContext.
     *
     * @param windowWidth Width of the application window.
     * @param windowHeight Height of the application window.
     * @return True if initialization succeeds.
     *
     * @code
     * if (!engine.Init(1280, 720)) return -1;
     * @endcode
     */
    [[nodiscard]] bool Init(int windowWidth, int windowHeight);

    /**
     * @brief Starts and runs the main game loop.
     *
     * @details
     * Handles input polling, game logic updates, rendering, and buffer swapping
     * until the window is closed or RequestQuit() is called.
     */
    void Run();

    /**
     * @brief Requests to exit the main loop after the current frame.
     *
     * @details
     * This flag is checked each frame to determine whether to continue running.
     * Typically used when pressing ESC or closing the window.
     */
    void RequestQuit();

    /**
     * @brief Returns the centralized engine context.
     *
     * @details
     * EngineContext provides access to subsystems like input, state, window, and renderer.
     *
     * @return Reference to the EngineContext.
     *
     * @code
     * EngineContext& ctx = engine.GetEngineContext();
     * if (ctx.inputManager->IsKeyPressed(KEY_SPACE)) { ... }
     * @endcode
     */
    [[nodiscard]] EngineContext& GetEngineContext() { return engineContext; }

private:
    /**
     * @brief Cleans up engine subsystems and releases resources.
     *
     * @details
     * Automatically called when the engine stops or is destroyed.
     */
    void Free() const;

    /**
     * @brief Registers all subsystems into the EngineContext.
     *
     * @details
     * Called internally by Init(). Connects managers to the context for global access.
     */
    void SetEngineContext();

    EngineContext engineContext;
    StateManager stateManager;
    WindowManager windowManager;
    InputManager inputManager;
    RenderManager renderManager;
    bool shouldRun = true;
};
