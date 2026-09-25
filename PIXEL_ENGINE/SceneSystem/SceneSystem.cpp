#include "SceneSystem.h"

SceneSystem::SceneSystem(float viewPortWidth, float viewPortHeight)
	:OurCameraManager(viewPortWidth, viewPortHeight)
	,OurSceneInfo{OurEntityManager.getEntityList(),
				  OurEntityManager.getActiveEntityID(),
				  OurCameraManager.getCameraList(),
				  OurCameraManager.getActiveCamera()}	// Must be initialized before creating SceneSystem
{
	this->viewPortWidth = viewPortWidth;
	this->viewPortHeight = viewPortHeight;
}

const SceneInfo& SceneSystem::getSceneInfo() const {
	return OurSceneInfo;
}

void SceneSystem::SceneUpdate(float viewPortWidth, float viewPortHeigh) {
	OurEntityManager.Update();
}
