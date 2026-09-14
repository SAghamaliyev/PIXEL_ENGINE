#include "SceneSystem.h"

void SceneSystem::SceneClearEntityList() {
	OurEntityManager.clearEntityList();
}

void SceneSystem::SceneCreateEntity(unsigned long long int MeshID, MaterialType Type, const string& Name) {
	OurEntityManager.createEntity(MeshID, Type, Name);
}


void SceneSystem::SceneDeleteEntity(unsigned int EntityID) {
	OurEntityManager.deactivateEntity(EntityID);
}

void SceneSystem::SceneDuplicateEntity(unsigned int EntityID) {
	OurEntityManager.duplicateEntity(EntityID);
}

void SceneSystem::ChangeColorEntity(unsigned int EntityID, const Color& targetColor) {
	OurEntityManager.changeColor(EntityID, targetColor);
}


void SceneSystem::ChangeNameEntity(unsigned int EntityID, const string& Name) {
	OurEntityManager.changeName(EntityID, Name);
}

void SceneSystem::ChangeMaterialEntity(unsigned int EntityID, MaterialType Type) {
	OurEntityManager.changeMaterial(EntityID, Type);
}

void SceneSystem::ChangeMeshEntity(unsigned int EntityID, uint64_t MeshID) {
	OurEntityManager.changeMesh(EntityID, MeshID);
}

void SceneSystem::ChangeTextureEntity(unsigned int EntityID, uint64_t TetxureID) {
	OurEntityManager.changeTexture(EntityID, TetxureID);
}

void SceneSystem::changeTranslateEntity(unsigned int EntityID, const glm::vec3& TranslateV) {
	OurEntityManager.changeTranslate(EntityID, TranslateV);
}

void SceneSystem::changeRotateEntity(unsigned int EntityID, const glm::vec3& RotateV) {
	OurEntityManager.changeRotate(EntityID, RotateV);
}

void SceneSystem::changeScaleEntity(unsigned int EntityID, const glm::vec3& ScaleV) {
	OurEntityManager.changeScale(EntityID, ScaleV);
}

void SceneSystem::activateColorEntity(unsigned int EntityID) {
	OurEntityManager.activateColor(EntityID);
}

void SceneSystem::deactivateColorEntity(unsigned int EntityID) {
	OurEntityManager.deactivateColor(EntityID);
}

//Transform& SceneSystem::getTransformInfoEntity(unsigned int EntityID) {
//	return OurEntityManager.getTransformInfo(EntityID);
//}

//EntityUnit& SceneSystem::SceneGetEntity(unsigned int EntityID) {
//	return OurEntityManager.getEntity(EntityID);
//}
