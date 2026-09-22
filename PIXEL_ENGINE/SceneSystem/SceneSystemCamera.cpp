#include "SceneSystem.h"

void SceneSystem::createSceneCamera() {
	OurCameraManager.createCamera();
}

void SceneSystem::deleteSceneCamera(unsigned int CameraID) {
	OurCameraManager.deleteCamera(CameraID);
}

void SceneSystem::changeFOVdegreeCamera(unsigned int CameraID, float FOVdegree) {
	OurCameraManager.changeFOVdegree(CameraID, FOVdegree);
}

void SceneSystem::changeNearCamera(unsigned int CameraID, float near) {
	OurCameraManager.changeNear(CameraID, near);
}

void SceneSystem::changeFarCamera(unsigned int CameraID, float far) {
	OurCameraManager.changeFar(CameraID, far);
}

void SceneSystem::changeSpeedCamera(unsigned int CameraID, float targetSpeed) {
	OurCameraManager.changeSpeed(CameraID, targetSpeed);
}

void  SceneSystem::changePosCamera(unsigned int CameraID, Movement direction) {
	OurCameraManager.changePos(CameraID, direction);
}