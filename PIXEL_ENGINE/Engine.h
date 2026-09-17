#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include "Core/InputSystem/InputSystem.h"
#include "RenderSystem/RenderSystem.h"
#include "SceneSystem/SceneSystem.h"
#include "src/Definitions.h"
#include "UI/EditorUI.h"
#include "Core/Logger/Logger.h"
#include "AssetSystem/AssetSystem.h"
#include <vector>

class Engine {
private:
	RenderSystem* OurRenderSystem;
	SceneSystem* OurSceneSystem;
	EditorUI* OurEditorUI;
	AssetSystem* OurAssetSystem;
	InputManager OurInputManager;
	GLFWwindow* OurWindow;


	void processEditorEvents();
	void visualizeEditorEvents();
public:
	bool initailize();
	void run();
	void terminate();

};
