#include "InputSystem.h"

#include <vector>

#include "Core/EventSystem/EventSystem.h"

using namespace std;

void InputManager::processEditorInput() {
    if (InputManager::isKeyJustPressed(InputManager::Key::Escape)) {
        EditorEvent event;
        event.type = EditorEventType::CloseWindow;
        EventSystem::pushEvent(event);
    }

    // G acts as a modifier key: G + 1 switches the gizmo to translate mode,
    // G + 2 to rotate mode, G + 3 to scale mode
    const bool is_G_active = InputManager::isKeyHeld(InputManager::Key::G)
                           || InputManager::isKeyJustPressed(InputManager::Key::G);

    if (is_G_active && InputManager::isKeyJustPressed(InputManager::Key::Num1)) {
        EditorEvent event;
        event.type = EditorEventType::SetGizmoTranslate;
        EventSystem::pushEvent(event);
    }

    if (is_G_active && InputManager::isKeyJustPressed(InputManager::Key::Num2)) {
        EditorEvent event;
        event.type = EditorEventType::SetGizmoRotate;
        EventSystem::pushEvent(event);
    }

    if (is_G_active && InputManager::isKeyJustPressed(InputManager::Key::Num3)) {
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

    if (InputManager::isMousePositionChanged()) {
        EditorEvent event;
        event.type = EditorEventType::SetCameraMouseEnabled;

        vector<float> answer = getMouseOffsets();
        event.info.mouseOffsetX = answer[0];
        event.info.mouseOffsetY = answer[1];
        EventSystem::pushEvent(event);
    }
}
