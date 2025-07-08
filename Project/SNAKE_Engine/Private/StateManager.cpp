
#include "StateManager.h"

void StateManager::ChangeState(std::unique_ptr<GameState> newState)
{
	nextState = std::move(newState);
}

void StateManager::Update(float dt, const EngineContext& engineContext)
{
	if (nextState != nullptr)
	{
		if (currentState!=nullptr)
			currentState->Free();
		currentState = std::move(nextState);
		currentState->Load();
		currentState->Init();
	}
	if (currentState != nullptr)
		currentState->Update(dt,engineContext);
}

void StateManager::Draw()
{
	if (currentState != nullptr)
		currentState->Draw();
}
