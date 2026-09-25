#include "SceneSystem.h"
using namespace std;

void SceneSystem::SceneClearEntityList() {
	OurEntityManager.clearEntityList();
}

void SceneSystem::SceneCreateEntity(unsigned long long int MeshID, MaterialType Type, const string& Name) {
	OurEntityManager.createEntity(MeshID, Type, Name);
}


void SceneSystem::SceneDeleteEntity(long EntityID) {
	OurEntityManager.deactivateEntity(EntityID);
}

void SceneSystem::SceneDuplicateEntity(long EntityID) {
	OurEntityManager.duplicateEntity(EntityID);
}

void SceneSystem::MakeActiveEntity(long EntityID) {

	try{
		OurEntityManager.makeActive(EntityID);
	}
	catch (const std::out_of_range& error){
		Logger::addLog(LOG_ERROR, error.what());
	}
}

void SceneSystem::ChangeColorEntity(long EntityID, const Color& targetColor) {
	OurEntityManager.changeColor(EntityID, targetColor);
}


void SceneSystem::ChangeNameEntity(long EntityID, const string& Name) {
	OurEntityManager.changeName(EntityID, Name);
}

void SceneSystem::ChangeMaterialEntity(long EntityID, MaterialType Type) {
	OurEntityManager.changeMaterial(EntityID, Type);
}

void SceneSystem::ChangeMeshEntity(long EntityID, uint64_t MeshID) {
	OurEntityManager.changeMesh(EntityID, MeshID);
}

void SceneSystem::ChangeTextureEntity(long EntityID, uint64_t TetxureID) {
	OurEntityManager.changeTexture(EntityID, TetxureID);
}

void SceneSystem::changeTranslateEntity(long EntityID, const glm::vec3& TranslateV) {
	OurEntityManager.changeTranslate(EntityID, TranslateV);

	auto& TempInfo = OurEntityManager.getEntity(EntityID).TransformInfo;
	OurTransformManager.TransformEntity(TempInfo);
}

void SceneSystem::changeRotateEntity(long EntityID, const glm::vec3& RotateV) {
	OurEntityManager.changeRotate(EntityID, RotateV);

	auto& TempInfo = OurEntityManager.getEntity(EntityID).TransformInfo;
	OurTransformManager.TransformEntity(TempInfo);
}

void SceneSystem::changeScaleEntity(long EntityID, const glm::vec3& ScaleV) {
	glm::vec3 clampedScale = ScaleV;
	if (clampedScale.x < 1.0f) {
		clampedScale.x = 1.0f;
	}
	if (clampedScale.y < 1.0f) {
		clampedScale.y = 1.0f;
	}
	if (clampedScale.z < 1.0f) {
		clampedScale.z = 1.0f;
	}

	OurEntityManager.changeScale(EntityID, clampedScale);

	auto& TempInfo = OurEntityManager.getEntity(EntityID).TransformInfo;
	OurTransformManager.TransformEntity(TempInfo);
}

void SceneSystem::activateColorEntity(long EntityID) {
	OurEntityManager.activateColor(EntityID);
}

void SceneSystem::deactivateColorEntity(long EntityID) {
	OurEntityManager.deactivateColor(EntityID);
}

const Transform& SceneSystem::getTransformInfoEntity(long EntityID) const {
	return OurEntityManager.getTransformInfo(EntityID);
}

//EntityUnit& SceneSystem::SceneGetEntity(unsigned int EntityID) {
//	return OurEntityManager.getEntity(EntityID);
//}
