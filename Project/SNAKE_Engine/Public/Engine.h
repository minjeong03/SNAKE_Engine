#pragma once

/**
 * @file Engine.h
 * @brief Unified include file for all core engine modules.
 *
 * Aggregates commonly used engine headers into a single location for convenience.
 * This is intended for source files (.cpp) that require broad access to engine features,
 * including subsystems such as rendering, input, game state, and math utilities.
 *
 * Recommended for use in application entry points, game state files, or quick prototypes.
 * Avoid including this in header files to prevent unnecessary coupling and compilation overhead.
 */

#include "SNAKE_Engine.h"
#include "GameState.h"
#include "EngineContext.h"
#include "InputManager.h"
#include "WindowManager.h"
#include "StateManager.h"
#include "ObjectManager.h"
#include "RenderManager.h"

#include "Texture.h"
#include "Shader.h"
#include "Transform.h"
#include "Material.h"
#include "Mesh.h"

#include "EngineTimer.h"
#include "CameraManager.h"
#include "Camera2D.h"

#include "../ThirdParty/glm/glm.hpp"
#include "../ThirdParty/glm/gtc/matrix_transform.hpp"
