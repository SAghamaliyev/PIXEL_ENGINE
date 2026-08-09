#include "SystemInputs.h"

void callSystemInputs(GLFWwindow* window) {
	InputManager::Update(window);

	if (InputManager::IsMouseButtonJustReleased(InputManager::MouseLeft)) {
		glfwSetWindowShouldClose(window, true);
	}
	
}
