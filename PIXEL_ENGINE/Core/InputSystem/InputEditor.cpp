#include "InputSystem.h"

void InputManager::InputEditor() {
	if (InputManager::IsKeyJustPressed(InputManager::Escape)) {
        EditorEvent event;
        event.type = EditorEventType::CloseWindow;
        EventSystem::pushEvent(event);
	}

    const bool isGActive = InputManager::IsKeyHeld(InputManager::G) || InputManager::IsKeyJustPressed(InputManager::G);

    if (isGActive && InputManager::IsKeyJustPressed(InputManager::Keys::Num1)) {
        EditorEvent event;
        event.type = EditorEventType::SetGizmoTranslate;
        EventSystem::pushEvent(event);
    }

    if (isGActive && InputManager::IsKeyJustPressed(InputManager::Keys::Num2)) {
        EditorEvent event;
        event.type = EditorEventType::SetGizmoRotate;
        EventSystem::pushEvent(event);
    }

    if (isGActive && InputManager::IsKeyJustPressed(InputManager::Keys::Num3)) {
        EditorEvent event;
        event.type = EditorEventType::SetGizmoScale;
        EventSystem::pushEvent(event);
    }

    if (InputManager::IsKeyHeld(InputManager::Keys::W)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMoveForward;
        EventSystem::pushEvent(event);
    }

    if (InputManager::IsKeyHeld(InputManager::Keys::S)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMoveBackward;
        EventSystem::pushEvent(event);
    }

    if (InputManager::IsKeyHeld(InputManager::Keys::D)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMoveRight;
        EventSystem::pushEvent(event);
    }

    if (InputManager::IsKeyHeld(InputManager::Keys::A)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMoveLeft;
        EventSystem::pushEvent(event);
    }

    if (InputManager::IsMouseButtonHeld(InputManager::MouseButtons::MouseRight)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMouseShouldChange;
        EventSystem::pushEvent(event);
    }

    if (InputManager::IsMouseButtonJustReleased(InputManager::MouseButtons::MouseRight)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMouseShouldStay;
        EventSystem::pushEvent(event);
    }

    if (InputManager::IsKeyHeld(InputManager::Keys::Space)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMoveUp;
        EventSystem::pushEvent(event);
    }

    if (InputManager::IsKeyHeld(InputManager::Keys::LeftShift)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMoveDown;
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
