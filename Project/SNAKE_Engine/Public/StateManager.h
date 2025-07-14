#pragma once

#include <memory>
#include "GameState.h"

struct EngineContext;

/**
 * @brief Handles game state transitions and lifecycle management.
 *
 * @details
 * StateManager controls the active game state and manages transitions
 * between different states (e.g., from TitleScreen to Gameplay).
 *
 * The manager holds two states:
 * - currentState: the active state currently being updated and drawn
 * - nextState: a queued state that will replace currentState at the next update
 *
 * Usage:
 * - Call ChangeState() to set the next state
 * - Call Update() and Draw() from the main loop
 *
 * This class ensures safe state transitions by delaying the switch until the start of a frame.
 *
 * @author Jinwoo Choi
 * @date 2025-07-08
 */
class StateManager
{
public:
    /**
     * @brief Returns the currently active game state.
     * @return Pointer to the current GameState.
     */
    [[nodiscard]] GameState* GetCurrentState() const;

    /**
     * @brief Queues a new game state to replace the current one.
     * @param newState A unique pointer to the next GameState.
     *
     * @details
     * The actual transition happens at the start of the next Update().
     * The current state will be safely freed before switching.
     */
    void ChangeState(std::unique_ptr<GameState> newState);

    /**
     * @brief Updates the current game state and handles state transitions.
     * @param dt Delta time since the last frame.
     * @param engineContext Shared engine systems.
     */
    void Update(float dt, const EngineContext& engineContext);

    /**
     * @brief Draws the current game state.
     * @param engineContext Rendering context.
     */
    void Draw(const EngineContext& engineContext);

    void Free(const EngineContext& engineContext);

private:
    /** The currently active state. */
    std::unique_ptr<GameState> currentState;

    /** The state to be switched to at the next update. */
    std::unique_ptr<GameState> nextState;
};
