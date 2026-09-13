#include "SystemInputs.h"

void callSystemInputs(GLFWwindow* window) {
	InputManager::Update(window);

	if (InputManager::IsKeyJustPressed(InputManager::Escape)) {
		glfwSetWindowShouldClose(window, true);
	}

	//if((InputManager::IsKeyJustPressed(InputManager::LeftControl) || 
	//	InputManager::IsKeyJustPressed(InputManager::RightControl)) &&
	//	InputManager::IsKeyJustPressed(InputManager::E)) {
	//	glfwSetWindowIcon; Event system should we make
	//}
	
}
