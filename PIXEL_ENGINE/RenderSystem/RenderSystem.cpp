#include "RenderSystem.h"

void RenderSystem::renderScene(const SceneInfo& OurSceneInfo) {

	// PreSettings for future render
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto Entity : OurSceneInfo.EntityList) {
		
		// Mesh to render
		MeshInfo OurMesh = OurMeshManager.getMesh(Entity.second.MeshID,
			Entity.second.isActive);

		// If isActive = false, it means this Entity is deactivated
		if (!OurMesh.isActive) {
			continue;
		}

		// Shader to render
		unsigned int OurShader = OurShaderManager.getShader(Entity.second.ShaderID);

		bool isColorActive = Entity.second.isColorActive;
		Color Temp = Entity.second.color;
		unsigned int OurTexture = OurTextureManager.getTexture(Entity.second.TextureID, Entity.second.isActive);

		int boolLocation = glGetUniformLocation(OurShader, "isColorActive");
		int ColorLocation = glGetUniformLocation(OurShader, "OurColor");
		int TextureLocation = glGetUniformLocation(OurShader, "OurTexture2D");



		glUseProgram(OurShader);
		// Устанавливаем флаг цвета
		glUniform1i(boolLocation, isColorActive);

		// Если цвет активен, применяем его
		if (isColorActive) {
			glUniform4f(ColorLocation, Temp.r, Temp.g, Temp.b, Temp.a);
		}

		// Привязываем текстуру к текстурному модулю 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, OurTexture);
		glUniform1i(TextureLocation, 0);

		glBindVertexArray(OurMesh.VAO);
		glDrawElements(GL_TRIANGLES, OurMesh.indexCount, GL_UNSIGNED_INT, 0);
	}
}