#include "InputSystem.h"

void InputManager::InputEditor() {
	if (InputManager::IsKeyJustPressed(InputManager::Escape)) {
        EditorEvent event;
        event.type = EditorEventType::CloseWindow;
        EventSystem::pushEvent(event);
	}
}
