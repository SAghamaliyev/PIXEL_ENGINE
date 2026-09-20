#include "SceneSystem.h"

SceneSystem::SceneSystem(float viewPortWidth, float viewPortHeight)
	:OurCameraManager(viewPortWidth, viewPortHeight)
	,OurSceneInfo{OurEntityManager.getEntityList(),OurCameraManager.getActiveCamera()}	// Must be initialized before creating SceneSystem
{
}

const SceneInfo& SceneSystem::getSceneInfo() const {
	return OurSceneInfo;
}

void SceneSystem::SceneUpdate() {
	OurEntityManager.Update();
}
