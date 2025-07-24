#pragma once
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"

/**
 * @brief Represents a unique pair of mesh and material for batching.
 *
 * @details
 * This struct is used as a key when grouping GameObjects that share both a Mesh and a Material.
 * Such groups can be rendered together efficiently using instancing.
 * Comparison and ordering operators are provided for use in maps and sets.
 */
struct InstanceBatchKey
{
    Mesh* mesh;         ///< Pointer to the mesh used by a group of objects.
    Material* material; ///< Pointer to the material shared by the same group.

    /**
     * @brief Equality comparison used in hash-based containers.
     */
    bool operator==(const InstanceBatchKey& other) const {
        return mesh == other.mesh && material == other.material;
    }

    /**
     * @brief Ordering comparison used in sorted containers like std::map.
     */
    bool operator<(const InstanceBatchKey& other) const {
        if (mesh != other.mesh)
            return mesh < other.mesh;
        return material < other.material;
    }
};

/**
 * @brief Custom hash function for InstanceBatchKey to support use in unordered_map.
 */
namespace std
{
    template<>
    struct hash<InstanceBatchKey>
    {
        std::size_t operator()(const InstanceBatchKey& key) const noexcept
        {
            return hash<Mesh*>()(key.mesh) ^ (hash<Material*>()(key.material) << 1);
        }
    };
}

/**
 * @brief Type alias for a two-level map used to group objects for instanced rendering.
 *
 * @details
 * Groups objects by:
 * - Render layer (int)
 * - Batch key (mesh + material)
 */
using InstancedBatchMap = std::unordered_map<int, std::unordered_map<InstanceBatchKey, std::vector<GameObject*>>>;
