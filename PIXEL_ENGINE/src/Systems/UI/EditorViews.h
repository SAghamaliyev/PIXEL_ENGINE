#pragma once

#include <cstdint>
#include <string>

#include <glm/glm.hpp>

#include "Core/Definitions/CommonDefinitions.h"
#include "Core/Definitions/MaterialDefinitions.h"
#include "Core/EventSystem/EventTypes.h"

struct EditorHierarchyView {
    long entityID = 0;
    std::string name;
};

struct EditorEntityView {
    long entityID = 0;
    uint64_t meshID = 0;
    uint64_t textureID = 0;
    std::string name;
    MaterialType shaderID = MaterialType::Default;
    Color color;
    bool colorEnabled = true;
    EditorVec3 position;
    EditorVec3 rotation;
    EditorVec3 scale = { 1.0f, 1.0f, 1.0f };
    glm::mat4 worldMatrix = glm::mat4(1.0f);
};
