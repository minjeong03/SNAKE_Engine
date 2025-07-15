#pragma once

#include <memory>
#include "GameState.h"

class SNAKE_Engine;
struct EngineContext;

/**
 * @brief Manages the current and next game states.
 *
 * @details
 * StateManager owns the active GameState and handles transitions to the next one.
 * It allows only one active state at a time, but supports scheduling a new state
 * to be swapped in at the beginning of the next update cycle.
 *
 * State transitions are deferred to avoid accessing deleted states during execution.
 *
 * This class is updated every frame by the engine.
 *
 * @code
 * engineContext.stateManager->ChangeState(std::make_unique<MainMenuState>());
 * engineContext.stateManager->Update(dt, engineContext);
 * engineContext.stateManager->Draw(engineContext);
 * @endcode
 */
class StateManager
{
    friend SNAKE_Engine;
public:
    /**
     * @brief Returns the currently active GameState.
     *
     * @details
     * Used to query the state currently being updated and rendered.
     * May return nullptr if no state is loaded.
     *
     * @return Raw pointer to the active GameState.
     */
    [[nodiscard]] GameState* GetCurrentState() const;

    /**
     * @brief Schedules a new GameState to be activated.
     *
     * @details
     * The switch does not happen immediately. Instead, the new state is stored internally
     * and the swap occurs at the start of the next call to Update().
     * This avoids modifying the state mid-frame.
     *
     * @param newState A unique_ptr to the new GameState to use next frame.
     *
     * @code
     * stateManager->ChangeState(std::make_unique<PlayState>());
     * @endcode
     */
    void ChangeState(std::unique_ptr<GameState> newState);

private:
    /**
     * @brief Updates the active state and performs pending transitions.
     *
     * @details
     * If a new state has been queued via ChangeState(), it is swapped in here.
     * Then the current state's Update() is called.
     *
     * @param dt Delta time since the last frame.
     * @param engineContext Engine services and system access.
     */
    void Update(float dt, const EngineContext& engineContext);

    /**
     * @brief Draws the currently active state.
     *
     * @details
     * Invokes Draw() on the current GameState, if any is loaded.
     *
     * @param engineContext Provides rendering and resource access.
     */
    void Draw(const EngineContext& engineContext);

    /**
     * @brief Releases memory of both current and next states.
     *
     * @details
     * Called when shutting down the engine to clean up any loaded GameStates.
     *
     * @param engineContext Used for cleanup calls to each GameState.
     */
    void Free(const EngineContext& engineContext);

    std::unique_ptr<GameState> currentState;
    std::unique_ptr<GameState> nextState;
};
