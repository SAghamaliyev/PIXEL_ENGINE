#include "SceneSystem.h"

SceneSystem::SceneSystem()
	:OurSceneInfo{OurEntityManager.getEntityList()}	// Must be initialized before creating SceneSystem
{
}

const SceneInfo& SceneSystem::getSceneInfo() const {
	return OurSceneInfo;
}

void SceneSystem::SceneUpdate() {
	OurEntityManager.Update();
}
