#pragma once
#include<memory>
#include "GameState.h"
struct EngineContext;
/**
 * @brief
 * @details
 * @author Jinwoo Choi
 * @date 2025-07-08
 */
class StateManager
{
public:
	GameState* GetCurrentState() const;
	void ChangeState(std::unique_ptr<GameState> newState);
	void Update(float dt, const EngineContext& engineContext);
	void Draw(const EngineContext& engineContext) const;
private:
	std::unique_ptr<GameState> currentState;
	std::unique_ptr<GameState> nextState;
};
