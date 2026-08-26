#include "SystemInputs.h"

void callSystemInputs(GLFWwindow* window) {
	InputManager::Update(window);

	if (InputManager::IsKeyJustPressed(InputManager::Escape)) {
		glfwSetWindowShouldClose(window, true);
	}
	
}
