#include "SceneSystem.h"


SceneSystem::SceneSystem()
	:OurSceneInfo{OurEntityManager.getEntityList()}
{
}

void SceneSystem::SceneCreateEntity(const string& Path, MaterialType Type, const string& Name) {
	OurEntityManager.createEntity(Path, Type, Name);
}

void SceneSystem::SceneDeleteEntity(unsigned int EntityID) {
	OurEntityManager.deactivateEntity(EntityID);
}

EntityUnit& SceneSystem::SceneGetEntity(unsigned int EntityID) {
	return OurEntityManager.getEntity(EntityID);
}

SceneInfo& SceneSystem::getSceneInfo() {
	return OurSceneInfo;
}

void SceneSystem::SceneUpdate() {
	OurEntityManager.Update();
}
