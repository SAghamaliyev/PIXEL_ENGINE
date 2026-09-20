#include "RenderSystem.h"

void RenderSystem::renderScene(const SceneInfo& OurSceneInfo) {

	// PreSettings for future render
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CameraUnit OurCamera = OurSceneInfo.OurActiveCamera;

	for (const auto& Entity : OurSceneInfo.EntityList) {
		
		// Elements for sending to Shader
		MeshInfo OurMesh = OurMeshManager.getMesh(Entity.second.MeshID,
			Entity.second.isActive);

		if (!OurMesh.isActive) {	// If isActive = false, it means this Entity is deactivated
			continue;
		}

		unsigned int OurShader = OurShaderManager.getShader(Entity.second.ShaderID);
		unsigned int OurTexture = OurTextureManager.getTexture(Entity.second.TextureID, Entity.second.isActive);
		bool isColorActive = Entity.second.isColorActive;
		Color Temp = Entity.second.color;


		// Locations of uniform attributes in Shader
		int isColorActiveLocation = glGetUniformLocation(OurShader, "isColorActive");
		int ColorLocation = glGetUniformLocation(OurShader, "OurColor");
		int TextureLocation = glGetUniformLocation(OurShader, "OurTexture2D");
		int TransMatrixLocation = glGetUniformLocation(OurShader, "TransMatrix");
		int ViewMatrixLocation = glGetUniformLocation(OurShader, "View");
		int ProjectionMatrixLocation = glGetUniformLocation(OurShader, "Projection");

		glUseProgram(OurShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, OurTexture);
		glBindVertexArray(OurMesh.VAO);

		// Send Color settings to Shader
		glUniform1i(isColorActiveLocation, isColorActive);
		glUniform4f(ColorLocation, Temp.r, Temp.g, Temp.b, Temp.a);

		// Send Texture to Shader
		glUniform1i(TextureLocation, 0);

		// Send Transform Matrix to Shader
		glUniformMatrix4fv(TransMatrixLocation, 1, GL_FALSE, value_ptr(Entity.second.TransformInfo.OurMatrix));
		glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, value_ptr(OurCamera.View));
		glUniformMatrix4fv(ProjectionMatrixLocation, 1, GL_FALSE, value_ptr(OurCamera.Projection));

		glDrawElements(GL_TRIANGLES, OurMesh.indexCount, GL_UNSIGNED_INT, 0);
	}
}