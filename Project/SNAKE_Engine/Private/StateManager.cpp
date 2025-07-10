
#include "StateManager.h"

#include "EngineContext.h"

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
			currentState->SystemFree(engineContext);
			currentState->SystemUnload(engineContext);
		}
		currentState = std::move(nextState);
		currentState->SystemLoad(engineContext);
		currentState->SystemInit(engineContext);
	}
	if (currentState != nullptr)
	{
		currentState->SystemUpdate(dt, engineContext);
	}
}

void StateManager::Draw(const EngineContext& engineContext)
{
	if (currentState != nullptr)
	{
		engineContext.renderManager->BeginFrame();
		currentState->SystemDraw(engineContext);
		engineContext.renderManager->EndFrame();
	}
}

void StateManager::Free(const EngineContext& engineContext)
{
	if (currentState != nullptr)
	{
		currentState->GetObjectManager().FreeAll();
		currentState->Free(engineContext);
		currentState->Unload(engineContext);
	}
}
