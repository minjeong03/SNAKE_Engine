#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>

class RenderLayerManager
{
public:
    static constexpr uint8_t MaxLayers = 16;

    std::optional<uint8_t> RegisterLayer(const std::string& name);

    std::optional<uint8_t> GetLayerID(const std::string& name) const;

    std::optional<std::string> GetLayerName(uint8_t id) const;

    uint8_t GetLayerCount() const { return static_cast<uint8_t>(m_nameToID.size()); }

private:
    std::unordered_map<std::string, uint8_t> m_nameToID;
    std::vector<std::string> m_idToName;
};
