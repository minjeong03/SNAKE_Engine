#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Camera2D.h"

/**
 * @brief Manages multiple cameras within a GameState.
 * Cameras are registered and referenced by string tags.
 * This manager owns all cameras and handles their lifetime.
 */
class CameraManager
{
public:
    CameraManager();
    /**
     * @brief Registers a new camera with the given tag.
     *        Overwrites existing camera if the tag already exists.
     */
    void RegisterCamera(const std::string& tag, std::unique_ptr<Camera2D> camera)
    {
        cameraMap[tag] = std::move(camera);
    }

    /**
     * @brief Returns the camera with the given tag.
     * @return Pointer to Camera2D, or nullptr if not found.
     */
    [[nodiscard]] Camera2D* GetCamera(const std::string& tag) const
    {
        auto it = cameraMap.find(tag);
        return (it != cameraMap.end()) ? it->second.get() : nullptr;
    }

    /**
     * @brief Sets the tag of the camera to be considered active.
     */
    void SetActiveCamera(const std::string& tag)
    {
        if (cameraMap.find(tag) != cameraMap.end())
        {
            activeCameraTag = tag;
        }
    }

    /**
     * @brief Returns the currently active camera.
     * @return Pointer to Camera2D, or nullptr if not found.
     */
    [[nodiscard]] Camera2D* GetActiveCamera() const
    {
        return GetCamera(activeCameraTag);
    }

    void SetScreenSizeForAll(int width, int height)
    {
        for (auto& [tag, cam] : cameraMap)
        {
            if (cam)
                cam->SetScreenSize(width, height);
        }
    }

    void SetScreenSize(const std::string& tag, int width, int height)
    {
        if (cameraMap[tag])
            cameraMap[tag]->SetScreenSize(width, height);
    }

    void Clear()
    {
        cameraMap.clear();
    }

private:
    std::unordered_map<std::string, std::unique_ptr<Camera2D>> cameraMap;
    std::string activeCameraTag = "main";
};
