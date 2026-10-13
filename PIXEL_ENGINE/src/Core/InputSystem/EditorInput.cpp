#include "InputSystem.h"

#include "Core/EventSystem/EventSystem.h"

void InputManager::processEditorInput() {
    if (InputManager::isKeyJustPressed(InputManager::Key::Escape)) {
        EditorEvent event;
        event.type = EditorEventType::CloseWindow;
        EventSystem::pushEvent(event);
    }

    const bool isGActive = InputManager::isKeyHeld(InputManager::Key::G)
        || InputManager::isKeyJustPressed(InputManager::Key::G);

    if (isGActive && InputManager::isKeyJustPressed(InputManager::Key::Num1)) {
        EditorEvent event;
        event.type = EditorEventType::SetGizmoTranslate;
        EventSystem::pushEvent(event);
    }

    if (isGActive && InputManager::isKeyJustPressed(InputManager::Key::Num2)) {
        EditorEvent event;
        event.type = EditorEventType::SetGizmoRotate;
        EventSystem::pushEvent(event);
    }

    if (isGActive && InputManager::isKeyJustPressed(InputManager::Key::Num3)) {
        EditorEvent event;
        event.type = EditorEventType::SetGizmoScale;
        EventSystem::pushEvent(event);
    }

    if (InputManager::isKeyHeld(InputManager::Key::W)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMoveForward;
        EventSystem::pushEvent(event);
    }

    if (InputManager::isKeyHeld(InputManager::Key::S)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMoveBackward;
        EventSystem::pushEvent(event);
    }

    if (InputManager::isKeyHeld(InputManager::Key::D)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMoveRight;
        EventSystem::pushEvent(event);
    }

    if (InputManager::isKeyHeld(InputManager::Key::A)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMoveLeft;
        EventSystem::pushEvent(event);
    }

    if (InputManager::isMouseButtonHeld(InputManager::MouseButton::MouseRight)) {
        EditorEvent event;
        event.type = EditorEventType::SetCameraMouseEnabled;
        event.info.cameraMouseEnabled = true;
        EventSystem::pushEvent(event);
    }

    if (InputManager::isMouseButtonJustReleased(InputManager::MouseButton::MouseRight)) {
        EditorEvent event;
        event.type = EditorEventType::SetCameraMouseEnabled;
        event.info.cameraMouseEnabled = false;
        EventSystem::pushEvent(event);
    }

    if (InputManager::isKeyHeld(InputManager::Key::Space)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMoveUp;
        EventSystem::pushEvent(event);
    }

    if (InputManager::isKeyHeld(InputManager::Key::LeftShift)) {
        EditorEvent event;
        event.type = EditorEventType::CameraMoveDown;
        EventSystem::pushEvent(event);
    }


    float xOffset = 0.0f;
    float yOffset = 0.0f;

    if (InputManager::isMousePositionChanged(xOffset, yOffset)) {
        EditorEvent event;
        event.type = EditorEventType::SetCameraMouseEnabled;
        event.info.mouseOffsetX = xOffset;
        event.info.mouseOffsetY = yOffset;
        EventSystem::pushEvent(event);
    }

}
