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
    DeleteObject,
    RenameObject,
    DuplicateObject,
    ClearScene,
    OpenScene,
    SaveScene,
    SaveSceneAs,
    ChangeEntityColor,
    ChangeEntityMaterial,
    ChangeEntityTransform,
    AddComponent,
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
    std::string path;
    std::string name;
    std::string message;
    MaterialType materialType = Default;
    Color color;
    EditorVec3 position;
    EditorVec3 rotation;
    EditorVec3 scale = { 1.0f, 1.0f, 1.0f };
    EditorComponentType componentType = EditorComponentType::RigidBody;
};

struct EditorEntityView {
    unsigned int entityID = 0;
    std::string path;
    std::string name;
    MaterialType materialType = Default;
    Color color;
};

// /FLAG This is the full list of event flags the UI is allowed to create.
inline const std::vector<EditorEventType>& getCreatableEditorEvents() {
    static const std::vector<EditorEventType> events = {
        EditorEventType::AddObject,
        EditorEventType::DeleteObject,
        EditorEventType::RenameObject,
        EditorEventType::DuplicateObject,
        EditorEventType::ClearScene,
        EditorEventType::OpenScene,
        EditorEventType::SaveScene,
        EditorEventType::SaveSceneAs,
        EditorEventType::ChangeEntityColor,
        EditorEventType::ChangeEntityMaterial,
        EditorEventType::ChangeEntityTransform,
        EditorEventType::AddComponent,
        EditorEventType::SubmitConsoleCommand,
        EditorEventType::ClearConsole,
        EditorEventType::ShowAbout
    };

    return events;
}
