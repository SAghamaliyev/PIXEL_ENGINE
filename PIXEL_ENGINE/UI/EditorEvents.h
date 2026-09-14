#pragma once

#include "../src/Definitions.h"

#include <string>
#include <vector>

struct EditorVec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

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
    ShowAbout
};

enum class EditorComponentType {
    RigidBody,
    Script,
    Collider,
    Light,
    Camera,
    AudioSource
};

struct EditorEvent {
    EditorEventType type = EditorEventType::AddObject;
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
    EditorVec3 position;
    EditorVec3 rotation;
    EditorVec3 scale = { 1.0f, 1.0f, 1.0f };
    EditorComponentType componentType = EditorComponentType::RigidBody;
};

struct EditorEntityView {
    unsigned int entityID = 0;
    uint64_t meshID = 0;
    uint64_t textureID = 0;
    std::string name;
    MaterialType ShaderID = Default;
    Color color;
    bool colorEnabled = true;
};

// /FLAG This is the full list of event flags the UI is allowed to create.
inline const std::vector<EditorEventType>& getCreatableEditorEvents() {
    static const std::vector<EditorEventType> events = {
        EditorEventType::AddObject,
        EditorEventType::RegisterObject,
        EditorEventType::DeleteObject,
        EditorEventType::RenameObject,
        EditorEventType::DuplicateObject,
        EditorEventType::ClearScene,
        EditorEventType::OpenScene,
        EditorEventType::SaveScene,
        EditorEventType::SaveSceneAs,
        EditorEventType::ChangeEntityColor,
        EditorEventType::ToggleEntityColor,
        EditorEventType::ChangeEntityMaterial,
        EditorEventType::Translate,
        EditorEventType::Rotate,
        EditorEventType::Scale,
        EditorEventType::AddComponent,
        EditorEventType::SubmitConsoleCommand,
        EditorEventType::ClearConsole,
        EditorEventType::ShowAbout
    };

    return events;
}
