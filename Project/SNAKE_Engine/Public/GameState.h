#pragma once
#include "CameraManager.h"
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
    [[nodiscard]] virtual ObjectManager& GetObjectManager() { return objectManager; }

    CameraManager& GetCameraManager() { return cameraManager; }
    Camera2D* GetActiveCamera() const { return cameraManager.GetActiveCamera(); }
    void SetActiveCamera(const std::string& tag) { cameraManager.SetActiveCamera(tag); }
protected:
    /**
     * @brief User-defined initialization logic. Override this to add game objects or initialize variables.
     */
    virtual void Init([[maybe_unused]] const EngineContext& engineContext) {}

    /**
     * @brief Called after Init() and all objects have been added. Override for late-stage setup.
     */
    virtual void LateInit([[maybe_unused]] const EngineContext& engineContext) {}

    /**
     * @brief User-defined per-frame update logic. Called before ObjectManager updates.
     * @param dt Delta time since last frame.
     * @param engineContext Shared access to engine systems.
     */
    virtual void Update(float dt, [[maybe_unused]] const EngineContext& engineContext) {}

    /**
     * @brief Called after ObjectManager updates. Use for post-update cleanup or deferred logic.
     * @param dt Delta time.
     * @param engineContext Engine context.
     */
    virtual void LateUpdate([[maybe_unused]] float dt, [[maybe_unused]] const EngineContext& engineContext) {}

    /**
     * @brief Load persistent assets before Init(). Override to load textures, sounds, etc.
     */
    virtual void Load([[maybe_unused]] const EngineContext& engineContext) {}

    /**
     * @brief User-defined rendering logic. Called after ObjectManager draws all objects.
     * @param engineContext Rendering context.
     */
    virtual void Draw([[maybe_unused]] const EngineContext& engineContext) { objectManager.DrawAll(engineContext,cameraManager.GetActiveCamera()); }

    /**
     * @brief Cleanup for any custom objects or systems created in Init().
     */
    virtual void Free([[maybe_unused]] const EngineContext& engineContext) {}

    /**
     * @brief Unload heavy or persistent resources loaded in Load().
     */
    virtual void Unload([[maybe_unused]] const EngineContext& engineContext) {}

    /**
     * @brief Re-initializes the state by calling SystemFree() and SystemInit().
     * @note Can be used to restart the state without reloading assets.
     */
    void Restart(const EngineContext& engineContext)
    {
        SystemFree(engineContext);
        SystemInit(engineContext);
    }

    /**
     * @brief Each state owns its own object manager, which handles game objects.
     */
    ObjectManager objectManager;
    CameraManager cameraManager;
private:
    /**
     * @brief Internal engine call to load persistent assets.
     */
    virtual void SystemLoad(const EngineContext& engineContext)
    {
        Load(engineContext);
    }

    /**
     * @brief Internal engine call to initialize the state.
     * Calls Init(), then initializes objects, then LateInit().
     */
    virtual void SystemInit(const EngineContext& engineContext)
    {
        Init(engineContext);
        objectManager.InitAll(engineContext);
        LateInit(engineContext);
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
     * @brief Internal engine call to clean up all game objects and user-defined systems.
     */
    virtual void SystemFree(const EngineContext& engineContext)
    {
        Free(engineContext);
        objectManager.FreeAll(engineContext);
    }

    /**
     * @brief Internal engine call to unload persistent assets.
     */
    virtual void SystemUnload(const EngineContext& engineContext)
    {
        Unload(engineContext);
    }
};
