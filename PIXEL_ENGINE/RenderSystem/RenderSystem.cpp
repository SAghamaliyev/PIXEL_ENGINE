#include "RenderSystem.h"

void RenderSystem::renderScene(const SceneInfo& OurSceneInfo) {
	
	for (auto Entity : OurSceneInfo.EntityList) {
		MeshInfo OurMesh = OurMeshManager.getMesh(Entity.second.path,
												  Entity.second.EntityID,
												  Entity.second.isActive);

		// If -2007 it means this Entity is deactivated
		if (OurMesh.indexcount == -2007) {
			continue;
		}
		
		unsigned int OurShader = OurShaderManager.getShader(Entity.second.MaterialID);

		glUseProgram(OurShader);
		glBindVertexArray(OurMesh.VAO);
		glDrawElements(GL_TRIANGLES, OurMesh.indexcount, GL_UNSIGNED_INT, 0);
	}
}
