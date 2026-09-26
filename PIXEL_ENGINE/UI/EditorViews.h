#pragma once

#include "../../Core/Definitions/SceneDefinitons.h"
#include "../../Core/EventSystem/EventTypes.h"
#include <string>
#include <cstdint>

struct EditorHierarchyView {
    long entityID = 0;
    std::string name;
};

struct EditorEntityView {
    long entityID = 0;
    uint64_t meshID = 0;
    uint64_t textureID = 0;
    std::string name;
    MaterialType ShaderID = Default;
    Color color;
    bool colorEnabled = true;
    EditorVec3 position;
    EditorVec3 rotation;
    EditorVec3 scale = { 1.0f, 1.0f, 1.0f };
    glm::mat4 worldMatrix = glm::mat4(1.0f);
};
