#pragma once

#include <cstdint>
#include <string>

#include "Core/Definitions/CommonDefinitions.h"
#include "Core/Definitions/MaterialDefinitions.h"

// This struct is used for passing position/scale/rotation.
// I use it instead of glm::vec3 so we don't need to include the GLM library.
struct EditorVec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

enum class EditorEventType {
    AddEntity,
    RegisterMesh,
    RegisterTexture,
    DeleteEntity,
    RenameEntity,
    DuplicateEntity,
    RequestEntityInfo,
    ChangeEntityColor,
    ToggleEntityColor,
    SetEntityMaterial,


    ClearScene,
    OpenScene,
    SaveScene,
    SaveSceneAs,

    Translate,
    Rotate,
    Scale,
    AssignTexture,
    SubmitConsoleCommand,
    ClearConsole,
    ShowAbout,
    CloseWindow,

    SetGizmoTranslate,
    SetGizmoRotate,
    SetGizmoScale,


    CameraMoveForward,
    CameraMoveBackward,
    CameraMoveLeft,
    CameraMoveRight,
    CameraMoveUp,
    CameraMoveDown,
    UpdateCameraRotation,
    SetCameraMouseEnabled
};

// This struct is used for providing additional details that must be passed
struct EditorEventData {
    long entityID = 0;
    long cameraID = 0;
    uint64_t meshID = 0;
    uint64_t textureID = 0;
    std::string path;
    std::string name;
    std::string message;
    MaterialType materialType = MaterialType::Default;
    Color color;
    bool isColorTintEnabled = false;
    bool isEmptyEntity = false;
    bool cameraMouseEnabled = false;
    EditorVec3 position = { 0.0f, 0.0f, 0.0f };
    EditorVec3 rotation = { 0.0f, 0.0f, 0.0f };
    EditorVec3 scale = { 1.0f, 1.0f, 1.0f };
    float mouseOffsetX;
    float mouseOffsetY;
};

// This is our final struct which we will analyze in Engine.cpp
struct EditorEvent {
    EditorEventType type;
    EditorEventData info;
};

