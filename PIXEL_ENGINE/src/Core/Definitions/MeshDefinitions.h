#pragma once

#include <cstddef>

struct MeshInfo {
    unsigned int vao = 0;          // Vertex Array Object ID
    unsigned int vboPosition = 0;  // Position Vertex Buffer Object ID
    unsigned int vboTexture = 0;    // Texture Coordinate Vertex Buffer Object ID
    unsigned int ebo = 0;          // Element (Index) Buffer Object ID
    std::size_t indexCount = 0;    // Number of indices to draw
    std::size_t textureCount = 0;  // Number of attached textures
};
