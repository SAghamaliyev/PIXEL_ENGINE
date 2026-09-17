#include "InputSystem.h"

void InputManager::InputEditor() {
	if (InputManager::IsKeyJustPressed(InputManager::Escape)) {
        EditorEvent event;
        event.type = EditorEventType::CloseWindow;
        EventSystem::pushEvent(event);
	}

    const bool isGActive = InputManager::IsKeyHeld(InputManager::G) || InputManager::IsKeyJustPressed(InputManager::G);

    if (isGActive && InputManager::IsKeyJustPressed(InputManager::Num1)) {
        EditorEvent event;
        event.type = EditorEventType::SetGizmoOperation;
        event.info.gizmoOperation = EditorGizmoOperation::Translate;
        EventSystem::pushEvent(event);
    }

    if (isGActive && InputManager::IsKeyJustPressed(InputManager::Num2)) {
        EditorEvent event;
        event.type = EditorEventType::SetGizmoOperation;
        event.info.gizmoOperation = EditorGizmoOperation::Rotate;
        EventSystem::pushEvent(event);
    }

    if (isGActive && InputManager::IsKeyJustPressed(InputManager::Num3)) {
        EditorEvent event;
        event.type = EditorEventType::SetGizmoOperation;
        event.info.gizmoOperation = EditorGizmoOperation::Scale;
        EventSystem::pushEvent(event);
    }
}
