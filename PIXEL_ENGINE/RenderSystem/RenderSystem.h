#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "ShaderManager/ShaderManager.h"
#include "MeshManager/MeshManager.h"


class RenderSystem {
public:
	RenderSystem();
	void renderScene();

private:
	MaterialType getShader(MaterialType Material);
	MeshInfo getMesh(const string& path, unsigned int MeshID);
};
