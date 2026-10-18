#pragma once

#include <cstdint>
#include <string>

#include <glm/glm.hpp>

#include "Core/Definitions/CommonDefinitions.h"
#include "Core/Definitions/MaterialDefinitions.h"

// Events which can be called in Engine's editor mode
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

// Data attached to an editor event. Each event type uses only the fields it needs
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
    glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
    float mouseOffsetX;
    float mouseOffsetY;
};

// This is our final struct which we will analyze in Engine.cpp
struct EditorEvent {
    EditorEventType type;
    EditorEventData info;
};

