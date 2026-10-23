#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "Core/Definitions/MeshDefinitions.h"

class MeshManager {
public:
    // Returns the mesh by ID(if it's active), loading it from the .bin file
    MeshInfo getMesh(uint64_t meshID, bool isActive);
    ~MeshManager();
private:
    std::unordered_map <uint64_t, MeshInfo> m_meshMap;
    void makeMesh(uint64_t meshID);

    // It's just a cube
    void makeDefaultMesh();

    // Reads mesh data from a .bin file
    void readBinaryMesh(uint64_t meshID,
                        std::vector<float>& vertices,
                        std::vector<unsigned int>& indices,
                        std::vector<float>& textures);
};
