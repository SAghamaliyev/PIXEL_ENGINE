#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include "Core/InputSystem/InputSystem.h"
#include "RenderSystem/RenderSystem.h"
#include "SceneSystem/SceneSystem.h"
#include "Core/Definitions/SceneDefinitons.h"
#include "UI/UISystem.h"
#include "Core/Logger/Logger.h"
#include "AssetSystem/AssetSystem.h"
#include <vector>

class Engine {
private:
	RenderSystem* OurRenderSystem;
	SceneSystem* OurSceneSystem;
	UISystem* OurUISystem;
	AssetSystem* OurAssetSystem;
	InputManager OurInputManager;
	GLFWwindow* OurWindow;
	int screenW = 0;
	int screenH = 0;


	void processEditorEvents();
	void visualizeEditorEvents();
public:
	bool initailize();
	void run();
	void terminate();

};
