#include "SceneSystem.h"

#include <algorithm>
#include <cctype>
#include <filesystem>

SceneSystem::SceneSystem()
	:OurSceneInfo{OurEntityManager.getEntityList()}
{
}

void SceneSystem::SceneClearEntityList() {
	OurEntityManager.clearEntityList();
}

void SceneSystem::SceneCreateEntity(unsigned long long int MeshID, MaterialType Type, const string& Name) {
	OurEntityManager.createEntity(MeshID, Type, Name);
}


void SceneSystem::SceneDeleteEntity(unsigned int EntityID) {
	OurEntityManager.deactivateEntity(EntityID);
}

EntityUnit& SceneSystem::SceneGetEntity(unsigned int EntityID) {
	return OurEntityManager.getEntity(EntityID);
}

const SceneInfo& SceneSystem::getSceneInfo() const {
	return OurSceneInfo;
}

void SceneSystem::ChangeColorEntity(unsigned int EntityID, Color targetColor) {
	OurEntityManager.changeColor(EntityID, targetColor);
}

void SceneSystem::SceneHasEntity(unsigned int EntityID) const {
	OurEntityManager.hasEntity(EntityID);
}

void SceneSystem::SceneDuplicateEntity(unsigned int EntityID) {
	OurEntityManager.duplicateEntity(EntityID);
}

void SceneSystem::SceneRenameEntity(unsigned int EntityID, const string& Name) {
	OurEntityManager.changeName(EntityID, Name);
}

void SceneSystem::SceneSetEntityMaterial(unsigned int EntityID, MaterialType Type) {
	OurEntityManager.changeMaterial(EntityID, Type);
}

void SceneSystem::SceneUpdate() {
	OurEntityManager.Update();
}
