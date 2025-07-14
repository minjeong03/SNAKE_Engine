#pragma once
#include "Mesh.h"
#include "Material.h"
#include <utility>
/**
 * @brief
 * @details
 * @author Jinwoo Choi
 * @date 2025-07-14
 */

struct InstanceBatchKey {
    Mesh* mesh;
    Material* material;

    bool operator==(const InstanceBatchKey& other) const {
        return mesh == other.mesh && material == other.material;
    }
};

// Hash specialization for unordered_map
namespace std {
    template<>
    struct hash<InstanceBatchKey> {
        std::size_t operator()(const InstanceBatchKey& key) const {
            return hash<Mesh*>()(key.mesh) ^ (hash<Material*>()(key.material) << 1);
        }
    };
}

using InstancedBatchMap = std::unordered_map<int, std::unordered_map<InstanceBatchKey, std::vector<GameObject*>>>;

