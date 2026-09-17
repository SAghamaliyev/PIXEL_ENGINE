#pragma once

#include "../../src/Definitions.h"

#include <string>
#include <vector>

// This struct is being used for passing position/scale/rotation
struct EditorVec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

// This enum class is being used for passing type of component of Entity
// (you can find it in UI by clicking on Entity)
enum class EditorComponentType {
    RigidBody,
    Script,
    Collider,
    Light,
    Camera,
    AudioSource
};

enum class EditorGizmoOperation {
    Translate,
    Rotate,
    Scale
};

// This enum class is being used for defining type of Event when you click the button 
// in UI
enum class EditorEventType {
    AddObject,
    RegisterObject,
    RegisterTexture,
    DeleteObject,
    RenameObject,
    DuplicateObject,
    ClearScene,
    OpenScene,
    SaveScene,
    SaveSceneAs,
    ChangeEntityColor,
    ToggleEntityColor,
    ChangeEntityMaterial,
    Translate,
    Rotate,
    Scale,
    AddComponent,
    AssignTexture,
    SubmitConsoleCommand,
    ClearConsole,
    ShowAbout,
    CloseWindow,
    SetGizmoOperation
};

// This struct is being used for giving details which we must pass
struct EditorTypeInfo {
    unsigned int entityID = 0;
    uint64_t meshID = 0;
    uint64_t textureID = 0;
    std::string path;
    std::string name;
    std::string message;
    MaterialType ShaderID = Default;
    Color color;
    bool colorEnabled = false;
    bool isEmptyEntity = false;
    EditorVec3 position = { 0.0f, 0.0f, 0.0f };
    EditorVec3 rotation = { 0.0f, 0.0f, 0.0f };
    EditorVec3 scale = { 1.0f, 1.0f, 1.0f };
    EditorComponentType componentType = EditorComponentType::RigidBody;
    EditorGizmoOperation gizmoOperation = EditorGizmoOperation::Translate;
};

// This is our final struct which we will analyze in Engine.cpp
struct EditorEvent {
    EditorEventType type;
    EditorTypeInfo info;
};

struct EditorEntityView {
    unsigned int entityID = 0;
    uint64_t meshID = 0;
    uint64_t textureID = 0;
    std::string name;
    MaterialType ShaderID = Default;
    Color color;
    bool colorEnabled = true;
    EditorVec3 position;
    EditorVec3 rotation;
    EditorVec3 scale = { 1.0f, 1.0f, 1.0f };
};
