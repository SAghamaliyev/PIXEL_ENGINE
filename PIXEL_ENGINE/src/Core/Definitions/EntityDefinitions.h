#pragma once

#include <cstdint>
#include <string>

#include <glm/glm.hpp>

#include "Core/Definitions/CommonDefinitions.h"
#include "Core/Definitions/MaterialDefinitions.h"

struct Transform {
    glm::mat4 matrix = glm::mat4(1.0f);      // Combined model transformation matrix
    glm::vec3 rotation = glm::vec3(0.0f);    // Euler rotation angles (in degrees)
    glm::vec3 translation = glm::vec3(0.0f); // Position in scene
    glm::vec3 scale = glm::vec3(1.0f);       // Scale factors along X, Y, Z axes
};

struct EntityUnit {
    long entityID = -1;
    long copyCount = 0;  // Number of copies/duplicates of this entity
    uint64_t meshID = 0;
    uint64_t textureID = 0;
    MaterialType materialType = MaterialType::Default; // Material/shader type is being used for rendering entities in different way(if needed)

    Color color;
    std::string name;
    Transform transform;

    bool isActive = true;             // Controls whether the entity is active and rendered
    bool isColorTintEnabled = false;  // Enables custom color tinting over base texture
};
