#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include "Input/SystemInputs.h"
#include "RenderSystem/RenderSystem.h"
#include "SceneSystem/SceneSystem.h"
#include "src/Definitions.h"
#include "UI/EditorUI.h"

class Engine {
private:
	RenderSystem* OurRenderSystem;
	SceneSystem* OurSceneSystem;
	EditorUI* OurEditorUI;
	GLFWwindow* OurWindow;

public:
	bool initailize();
	void run();
	void terminate();

};