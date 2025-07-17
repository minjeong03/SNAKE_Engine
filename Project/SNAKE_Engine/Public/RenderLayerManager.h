#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <array>

#include "Debug.h"

/**
 * @brief Manages custom render layer names and their numeric IDs.
 *
 * This class allows users to define up to MAX_LAYERS named render layers
 * and retrieve their numeric ID for use in RenderManager.
 *
 * Accessible from: RenderManager
 *
 * Example usage:
 * @code
 * renderManager.RegisterRenderLayer("UI");
 * uint8_t uiLayer = renderManager.GetLayerManager().GetLayerID("UI").value_or(0);
 * @endcode
 */
class RenderLayerManager
{
    friend RenderManager;
public:
    static constexpr uint8_t MAX_LAYERS = 16; ///< Maximum number of layers that can be registered.

    /**
     * @brief Retrieves the numeric ID for a registered layer name.
     *
     * @param name The string name of the layer.
     * @return Optional numeric ID (0-15), or nullopt if not found.
     */
    std::optional<uint8_t> GetLayerID(const std::string& name) const
    {
        auto it = nameToID.find(name);
        if (it != nameToID.end())
            return it->second;
        return std::nullopt;
    }

    /**
     * @brief Gets the registered name of a layer given its ID.
     *
     * @param id Numeric ID of the layer (0-15).
     * @return Reference to the registered name string.
     */
    const std::string& GetLayerName(uint8_t id) const
    {
        return idToName.at(id);
    }

private:

    /**
     * @brief Registers a named render layer without returning its ID.
     *
     * @details
     * If the name already exists, a warning is logged and registration is skipped.
     * If the number of layers exceeds MAX_LAYERS, an error is logged and the function returns early.
     *
     * @param name The name of the render layer to register.
     */
    void RegisterLayer(const std::string& name)
    {
        auto it = nameToID.find(name);
        if (it != nameToID.end())
        {
            SNAKE_WRN("Already have '" << name << "' tag.\n");
            return;
        }

        if (nextID >= MAX_LAYERS)
        {
            SNAKE_ERR("Already have max count of layers.\n");
            return;
        }

        nameToID[name] = nextID;
        idToName[nextID] = name;
        nextID++;
    }

    std::unordered_map<std::string, uint8_t> nameToID;
    std::array<std::string, MAX_LAYERS> idToName;
    uint8_t nextID = 0;
};
