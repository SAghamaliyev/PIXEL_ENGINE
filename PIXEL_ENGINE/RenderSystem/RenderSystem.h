#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "ShaderManager/ShaderManager.h"
#include "MeshManager/MeshManager.h"
#include "../SceneSystem/SceneSystem.h"
#include "TextureManager/TextureManager.h"
#include "TransformationManager/TransformationManager.h"


class RenderSystem {
private:
	ShaderManager OurShaderManager;
	MeshManager OurMeshManager;
	TextureManager OurTextureManager;
	TransformationManager OurTransformationManager;
	//MaterialType getShader(MaterialType Material);
	//MeshInfo getMesh(const string& path, unsigned int MeshID);
public:
	RenderSystem() = default;
	void renderScene(const SceneInfo& OurSceneInfo);
};
