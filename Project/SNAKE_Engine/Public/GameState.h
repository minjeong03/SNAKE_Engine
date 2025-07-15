#pragma once
#include "CameraManager.h"
#include "ObjectManager.h"

class StateManager;
struct EngineContext;

/**
 * @brief Abstract base class representing a game state or scene.
 *
 * A GameState represents a self-contained mode in the game such as title screen, gameplay, or pause.
 * It defines a full lifecycle: Load -> Init -> Update/Draw -> Free -> Unload.
 * Each state manages its own ObjectManager and CameraManager.
 *
 * Override protected methods to implement the specific logic of your game state.
 */
class GameState
{
    friend StateManager;

public:
    /**
     * @brief Virtual destructor to allow safe cleanup of derived states.
     */
    virtual ~GameState() = default;

    /**
     * @brief Returns the object manager used by this state.
     *
     * Provides access to game object creation, update, and rendering.
     *
     * @return Reference to the ObjectManager.
     */
    [[nodiscard]] virtual ObjectManager& GetObjectManager() { return objectManager; }

    /**
     * @brief Returns the camera manager for this state.
     *
     * Provides access to registered 2D cameras and camera switching.
     *
     * @return Reference to the CameraManager.
     */
    CameraManager& GetCameraManager() { return cameraManager; }

    /**
     * @brief Returns the currently active camera.
     *
     * @return Pointer to the active Camera2D, or nullptr if none is set.
     */
    Camera2D* GetActiveCamera() const { return cameraManager.GetActiveCamera(); }

    /**
     * @brief Sets the active camera by tag.
     *
     * @param tag A string key matching a previously registered camera.
     */
    void SetActiveCamera(const std::string& tag) { cameraManager.SetActiveCamera(tag); }

protected:
    /**
     * @brief Called once at the start of the state's lifetime.
     *
     * Override to create game objects and initialize state variables.
     *
     * @param engineContext Provides access to engine-wide systems.
     */
    virtual void Init([[maybe_unused]] const EngineContext& engineContext) {}

    /**
     * @brief Called after Init and after all game objects are initialized.
     *
     * Useful for referencing other objects or setting up systems that depend on them.
     *
     * @param engineContext Engine-wide context.
     */
    virtual void LateInit([[maybe_unused]] const EngineContext& engineContext) {}

    /**
     * @brief Per-frame update logic for the state.
     *
     * Runs before ObjectManager updates its objects.
     *
     * @param dt Delta time since last frame.
     * @param engineContext Engine services and data access.
     */
    virtual void Update(float dt, [[maybe_unused]] const EngineContext& engineContext) {}

    /**
     * @brief Called after all objects have been updated.
     *
     * Useful for post-processing, cleanup, or deferred logic.
     *
     * @param dt Delta time.
     * @param engineContext Engine context.
     */
    virtual void LateUpdate([[maybe_unused]] float dt, [[maybe_unused]] const EngineContext& engineContext) {}

    /**
     * @brief Loads persistent assets like textures or audio.
     *
     * Called before Init(). Override to preload large assets.
     *
     * @param engineContext Used for resource loading.
     */
    virtual void Load([[maybe_unused]] const EngineContext& engineContext) {}

    /**
     * @brief Draws the state to the screen.
     *
     * Default implementation draws all registered objects using the active camera.
     *
     * @param engineContext Provides access to renderer and camera.
     */
    virtual void Draw([[maybe_unused]] const EngineContext& engineContext)
    {
        objectManager.DrawAll(engineContext, cameraManager.GetActiveCamera());
    }

    /**
     * @brief Frees memory created in Init().
     *
     * Called when the state is being shut down or restarted.
     *
     * @param engineContext For resource cleanup.
     */
    virtual void Free([[maybe_unused]] const EngineContext& engineContext) {}

    /**
     * @brief Unloads persistent assets previously loaded in Load().
     *
     * @param engineContext Used to access file/resource systems.
     */
    virtual void Unload([[maybe_unused]] const EngineContext& engineContext) {}

    /**
     * @brief Restarts the state without unloading its assets.
     *
     * Calls Free and Init again.
     *
     * @param engineContext Engine context passed into Init and Free.
     *
     * Example usage:
     * @code
     * engineContext.stateManager->GetCurrentState()->Restart(engineContext);
     * @endcode
     */
    void Restart(const EngineContext& engineContext)
    {
        SystemFree(engineContext);
        SystemInit(engineContext);
    }

    ObjectManager objectManager;
    CameraManager cameraManager;

private:
    /**
     * @brief Called internally by the engine to begin loading phase.
     */
    virtual void SystemLoad(const EngineContext& engineContext)
    {
        Load(engineContext);
    }

    /**
     * @brief Called internally to initialize the state and its objects.
     */
    virtual void SystemInit(const EngineContext& engineContext)
    {
        Init(engineContext);
        objectManager.InitAll(engineContext);
        LateInit(engineContext);
    }

    /**
     * @brief Called every frame to update the state and its objects.
     */
    virtual void SystemUpdate(float dt, const EngineContext& engineContext)
    {
        Update(dt, engineContext);
        objectManager.UpdateAll(dt, engineContext);
        LateUpdate(dt, engineContext);
    }

    /**
     * @brief Called during shutdown or Restart to clean up state.
     */
    virtual void SystemFree(const EngineContext& engineContext)
    {
        Free(engineContext);
        objectManager.FreeAll(engineContext);
    }

    /**
     * @brief Called after shutdown to unload persistent assets.
     */
    virtual void SystemUnload(const EngineContext& engineContext)
    {
        Unload(engineContext);
    }
};
