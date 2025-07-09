#pragma once
#include "ObjectManager.h"

struct EngineContext;
/**
 * @brief
 * @details
 * @author Jinwoo Choi
 * @date 2025-07-08
 */
class GameState
{
public:

	virtual void Load() {} /** Load heavy files(music, textures etc...) that don't need reloading*/

	/** Initialize transforms, etc... */
	virtual void Init()
	{
		objectManager.InitAll();
	} 

	virtual void Update(float dt, const EngineContext& engineContext)
	{
		objectManager.UpdateAll(dt,engineContext);
	}
	virtual void Draw(const EngineContext& engineContext)
	{
		objectManager.DrawAll(engineContext);
	}

	virtual void Free() {} /** Free things that loaded from Init()*/
	virtual void Unload() {} /** Unload things that loaded from Load()*/
	virtual ~GameState() {}

	virtual ObjectManager& GetObjectManager() { return objectManager; }

protected:
	ObjectManager objectManager;
};