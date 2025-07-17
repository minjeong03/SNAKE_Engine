#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <array>

class RenderLayerManager
{
public:
    static constexpr uint8_t MAX_LAYERS = 16;

    std::optional<uint8_t> RegisterLayer(const std::string& name)
    {
        auto it = nameToID.find(name);
        if (it != nameToID.end())
            return it->second;

        if (nextID >= MAX_LAYERS)
            return std::nullopt;

        nameToID[name] = nextID;
        idToName[nextID] = name;
        return nextID++;
    }

    std::optional<uint8_t> GetLayerID(const std::string& name) const
    {
        auto it = nameToID.find(name);
        if (it != nameToID.end())
            return it->second;
        return std::nullopt;
    }

    const std::string& GetLayerName(uint8_t id) const
    {
        return idToName.at(id);
    }

private:
    std::unordered_map<std::string, uint8_t> nameToID;
    std::array<std::string, MAX_LAYERS> idToName;
    uint8_t nextID = 0;
};

