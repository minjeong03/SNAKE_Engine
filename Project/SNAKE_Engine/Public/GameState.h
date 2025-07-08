#pragma once

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
	virtual void Init() {} /** Initialize transforms, etc... */

	virtual void Update(float dt, const EngineContext& engineContext) {}
	virtual void Draw() {}
	virtual void Free() {} /** Free things that loaded from Init()*/
	virtual void Unload() {} /** Unload things that loaded from Load()*/
	virtual ~GameState() {}
};