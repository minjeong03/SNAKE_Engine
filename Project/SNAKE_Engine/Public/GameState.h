#pragma once
#include "ObjectManager.h"

struct EngineContext;

/**
 * @brief Base class for defining game states (such as menus, gameplay, pause, etc.)
 *
 * @details
 * GameState is an abstract base class that encapsulates the lifecycle of a particular state in the game.
 * Typical usage involves subclassing GameState for specific states (e.g., TitleState, PlayState) and overriding the lifecycle methods:
 * - Load(): Load assets that are persistent during the state's lifetime
 * - Init(): Initialize objects and scene-specific data
 * - Update(): Per-frame logic update
 * - Draw(): Rendering
 * - Free(): Clean up objects created in Init
 * - Unload(): Release heavy resources loaded in Load
 *
 * Each GameState owns its own ObjectManager instance to isolate game objects per state.
 * This helps with modularity and memory management.
 *
 * @author Jinwoo Choi
 * @date 2025-07-08
 */
class GameState
{
public:

	/**
	 * @brief Load persistent or heavy assets (e.g. textures, music) before state initialization.
	 * @note Override in derived classes to load resources that don't need to be reloaded every time.
	 */
	virtual void Load() {}

	/**
	 * @brief Initialize game objects and state-specific setup.
	 * @details Calls InitAll() on the internal ObjectManager.
	 */
	virtual void Init()
	{
		objectManager.InitAll();
	}

	/**
	 * @brief Per-frame logic update.
	 * @param dt Delta time since last frame.
	 * @param engineContext Engine-wide access to shared systems.
	 */
	virtual void Update(float dt, const EngineContext& engineContext)
	{
		objectManager.UpdateAll(dt, engineContext);
	}

	/**
	 * @brief Draw all game objects associated with this state.
	 * @param engineContext Engine context passed to each object's Draw().
	 */
	virtual void Draw(const EngineContext& engineContext)
	{
		objectManager.DrawAll(engineContext);
	}

	/**
	 * @brief Clean up game objects and temporary data created in Init().
	 * @note Override in derived classes if additional cleanup is needed.
	 */
	virtual void Free() {}

	
	/**
	 * @brief Unload persistent assets loaded in Load().
	 */
	virtual void Unload() {}

	/**
	 * @brief Virtual destructor.
	 */
	virtual ~GameState() {}

	/**
	 * @brief Access the state's internal ObjectManager.
	 * @return Reference to ObjectManager instance.
	 */
	virtual ObjectManager& GetObjectManager() { return objectManager; }

protected:
	/**
	 * @brief Each state has its own object manager.
	 */
	ObjectManager objectManager;
};
