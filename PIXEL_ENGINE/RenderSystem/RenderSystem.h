#pragma once
#include "ShaderManager/ShaderManager.h"
#include "MeshManager/MeshManager.h"
#include "TextureManager/TextureManager.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

class RenderSystem {
private:
	ShaderManager OurShaderManager;
	MeshManager OurMeshManager;
	TextureManager OurTextureManager;
public:
	void renderScene(const SceneInfo& OurSceneInfo);
};
