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
        event.type = EditorEventType::SetGizmoTranslate;
        EventSystem::pushEvent(event);
    }

    if (isGActive && InputManager::IsKeyJustPressed(InputManager::Num2)) {
        EditorEvent event;
        event.type = EditorEventType::SetGizmoRotate;
        EventSystem::pushEvent(event);
    }

    if (isGActive && InputManager::IsKeyJustPressed(InputManager::Num3)) {
        EditorEvent event;
        event.type = EditorEventType::SetGizmoScale;
        EventSystem::pushEvent(event);
    }

    if (InputManager::IsKeyHeld(InputManager::W)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMoveForward;
        EventSystem::pushEvent(event);
    }

    if (InputManager::IsKeyHeld(InputManager::S)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMoveBackward;
        EventSystem::pushEvent(event);
    }
    
    float xpos = 0.0f;
    float ypos = 0.0f;

    if (InputManager::isMousePositionChanged(xpos, ypos)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMouseChange;
        event.info.xpos = xpos;
        event.info.ypos = ypos;
        EventSystem::pushEvent(event);
    }

}
