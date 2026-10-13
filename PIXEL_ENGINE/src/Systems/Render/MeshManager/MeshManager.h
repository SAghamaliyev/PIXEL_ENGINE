#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "Core/Definitions/MeshDefinitions.h"

class MeshManager {
public:
    MeshInfo getMesh(uint64_t meshID, bool isActive);
    ~MeshManager();
private:
    std::unordered_map <uint64_t, MeshInfo> m_meshMap;
    void makeMesh(uint64_t meshID);

    // Read mesh data from a .bin file
    void readBinaryMesh(uint64_t meshID,
        std::vector<float>& vertices,
        std::vector<unsigned int>& indices,
        std::vector<float>& textures);
};
