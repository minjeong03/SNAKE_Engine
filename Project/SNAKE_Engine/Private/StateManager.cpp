#include "StateManager.h"
#include "GameState.h"
#include "EngineContext.h"
#include "WindowManager.h"
#include "RenderManager.h"
#include "SNAKE_Engine.h"
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
			engineContext.soundManager->ControlAll(SoundManager::SoundControlType::Stop);
		}
		engineContext.inputManager->Clear();
		currentState = std::move(nextState);
		currentState->SystemLoad(engineContext);
		currentState->SystemInit(engineContext);
		currentState->GetCameraManager().SetScreenSizeForAll(
engineContext.windowManager->GetWidth(), engineContext.windowManager->GetHeight());
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
		currentState->Draw(engineContext);
		engineContext.renderManager->FlushDrawCommands(engineContext);
                if (engineContext.engine->ShouldRenderDebugDraws())
		    engineContext.renderManager->FlushDebugLineDrawCommands(engineContext);
	}
}

void StateManager::Free(const EngineContext& engineContext)
{
	if (currentState != nullptr)
	{
		currentState->SystemFree(engineContext);
		currentState->SystemUnload(engineContext);
	}
}
