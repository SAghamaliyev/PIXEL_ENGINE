#include "RenderSystem.h"

void RenderSystem::renderScene(const SceneInfo& OurSceneInfo) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	for (auto Entity : OurSceneInfo.EntityList) {
		MeshInfo OurMesh = OurMeshManager.getMesh(Entity.second.path,
												  Entity.second.EntityID,
												  Entity.second.isActive);

		// If -2007 it means this Entity is deactivated
		if (OurMesh.indexcount == -2007) {
			continue;
		}
		
		unsigned int OurShader = OurShaderManager.getShader(Entity.second.MaterialID);

		int ColorLocation = glGetUniformLocation(OurShader, "OurColor");
		glUseProgram(OurShader);
		Color Temp = Entity.second.color;
		glUniform4f(ColorLocation, Temp.r, Temp.g, Temp.b, Temp.a);

		glBindVertexArray(OurMesh.VAO);
		glDrawElements(GL_TRIANGLES, OurMesh.indexcount, GL_UNSIGNED_INT, 0);
	}
}
