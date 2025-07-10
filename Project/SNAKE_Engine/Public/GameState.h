#pragma once
#include "ObjectManager.h"

class StateManager;
struct EngineContext;

/**
 * @brief Base class for defining game states (such as menus, gameplay, pause, etc.)
 *
 * @details
 * GameState is an abstract base class that represents a self-contained mode or scene in the game,
 * such as main menu, gameplay, pause screen, etc. It provides a structured lifecycle that the engine
 * can manage consistently:
 *
 * - Load(): Load persistent resources (e.g., textures, music)
 * - Init(): Set up game objects and state-specific logic
 * - Update(): Per-frame logic
 * - Draw(): Rendering
 * - Free(): Cleanup objects created in Init
 * - Unload(): Release persistent assets
 *
 * Each state manages its own ObjectManager and is expected to be subclassed by specific game states
 * like TitleState, Level1, PauseMenu, etc.
 *
 * The engine calls the System-prefixed methods internally to control flow.
 * Users should override the protected lifecycle methods to define custom behavior.
 *
 * @author Jinwoo Choi
 * @date 2025-07-08
 */
class GameState
{
    friend StateManager;

public:
    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~GameState() = default;

    /**
     * @brief Access the state's internal ObjectManager.
     * @return Reference to the ObjectManager instance used by this state.
     */
    virtual ObjectManager& GetObjectManager() { return objectManager; }

protected:
    /**
     * @brief User-defined initialization logic. Override this to add game objects or initialize variables.
     */
    virtual void Init() {}

    /**
     * @brief Called after Init() and all objects have been added. Override for late-stage setup.
     */
    virtual void LateInit() {}

    /**
     * @brief User-defined per-frame update logic. Called before ObjectManager updates.
     * @param dt Delta time since last frame.
     * @param engineContext Shared access to engine systems.
     */
    virtual void Update(float dt, const EngineContext& engineContext) {}

    /**
     * @brief Called after ObjectManager updates. Use for post-update cleanup or deferred logic.
     * @param dt Delta time.
     * @param engineContext Engine context.
     */
    virtual void LateUpdate(float dt, const EngineContext& engineContext) {}

    /**
     * @brief Load persistent assets before Init(). Override to load textures, sounds, etc.
     */
    virtual void Load() {}

    /**
     * @brief User-defined rendering logic. Called after ObjectManager draws all objects.
     * @param engineContext Rendering context.
     */
    virtual void Draw(const EngineContext& engineContext) {}

    /**
     * @brief Cleanup for any custom objects or systems created in Init().
     */
    virtual void Free() {}

    /**
     * @brief Unload heavy or persistent resources loaded in Load().
     */
    virtual void Unload() {}

    /**
     * @brief Re-initializes the state by calling SystemFree() and SystemInit().
     * @note Can be used to restart the state without reloading assets.
     */
    void Restart()
    {
        SystemFree();
        SystemInit();
    }

    /**
     * @brief Each state owns its own object manager, which handles game objects.
     */
    ObjectManager objectManager;

private:
    /**
     * @brief Internal engine call to load persistent assets.
     */
    virtual void SystemLoad()
    {
        Load();
    }

    /**
     * @brief Internal engine call to initialize the state.
     * Calls Init(), then initializes objects, then LateInit().
     */
    virtual void SystemInit()
    {
        Init();
        objectManager.InitAll();
        LateInit();
    }

    /**
     * @brief Internal engine call to update the state each frame.
     * Calls Update(), updates objects, then calls LateUpdate().
     *
     * @param dt Delta time.
     * @param engineContext Engine context.
     */
    virtual void SystemUpdate(float dt, const EngineContext& engineContext)
    {
        Update(dt, engineContext);
        objectManager.UpdateAll(dt, engineContext);
        LateUpdate(dt, engineContext);
    }

    /**
     * @brief Internal engine call to draw the state.
     * Draws user content first, then objectManager content.
     *
     * @param engineContext Rendering context.
     */
    virtual void SystemDraw(const EngineContext& engineContext)
    {
        Draw(engineContext);
        objectManager.DrawAll(engineContext);
    }

    /**
     * @brief Internal engine call to clean up all game objects and user-defined systems.
     */
    virtual void SystemFree()
    {
        Free();
        objectManager.FreeAll();
    }

    /**
     * @brief Internal engine call to unload persistent assets.
     */
    virtual void SystemUnload()
    {
        Unload();
    }
};
