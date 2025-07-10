#pragma once

/**
 * @file Engine.h
 * @brief Centralized header collection for engine-wide includes
 *
 * @details
 * This file aggregates frequently used engine headers into a single include point.
 * It is intended to simplify includes across files that depend on core systems,
 * such as engine loop, input, windowing, game state, and object management.
 *
 * Use this in source files that require broad access to engine components.
 * Avoid using this in header files to prevent unnecessary dependencies.
 *
 * @author Jinwoo Choi
 * @date 2025-07-08
 */


#include "SNAKE_Engine.h"
#include "GameState.h"
#include "EngineContext.h"
#include "InputManager.h"
#include "WindowManager.h"
#include "StateManager.h"
#include "ObjectManager.h"
#include "RenderManager.h"

#include "../ThirdParty/glm/glm.hpp"
