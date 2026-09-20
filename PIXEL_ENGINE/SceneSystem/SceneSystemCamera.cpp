#include "SceneSystem.h"

void SceneSystem::createSceneCamera() {
	OurCameraManager.createCamera();
}

void SceneSystem::deleteSceneCamera(unsigned int CameraID) {
	OurCameraManager.deleteCamera(CameraID);
}

void SceneSystem::setSceneViewPort(float viewPortWidth, float viewPortHeight) {
	OurCameraManager.setViewPort(viewPortWidth, viewPortHeight);
}

void SceneSystem::changeTranslateCamera(unsigned int CameraID, glm::vec3 TransV) {
	OurCameraManager.setTransV(CameraID, TransV);

	auto& it = OurCameraManager.getCameraMatrix(CameraID);
	OurTransformManager.ReTransformCamera(it, TransV);
}

void SceneSystem::setFOVdegreeCamera(unsigned int CameraID, float FOVdegree) {
	OurCameraManager.setFOVdegree(CameraID, FOVdegree);
}

void SceneSystem::setNearCamera(unsigned int CameraID, float near) {
	OurCameraManager.setNear(CameraID, near);
}

void SceneSystem::setFarCamera(unsigned int CameraID, float far) {
	OurCameraManager.setFar(CameraID, far);
}