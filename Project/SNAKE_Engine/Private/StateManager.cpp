
#include "StateManager.h"

GameState* StateManager::GetCurrentState() const
{
	return currentState ? currentState.get() : nullptr;
}

void StateManager::ChangeState(std::unique_ptr<GameState> newState)
{
	nextState = std::move(newState);
}

void StateManager::Update(float dt, const EngineContext& engineContext)
{
	if (nextState != nullptr)
	{
		if (currentState != nullptr)
		{
			currentState->SystemFree();
			currentState->SystemUnload();
		}
		currentState = std::move(nextState);
		currentState->SystemLoad();
		currentState->SystemInit();
	}
	if (currentState != nullptr)
	{
		currentState->SystemUpdate(dt, engineContext);
	}
}

void StateManager::Draw(const EngineContext& engineContext) const
{
	if (currentState != nullptr)
		currentState->SystemDraw(engineContext);
}
