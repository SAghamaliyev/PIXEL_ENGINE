#include "CameraManager.h"
#include <string>

#define CameraDefault 0
using namespace std;

CameraManager::CameraManager(float viewPortWidth, float viewPortHeight)
	: viewPortWidth(viewPortWidth), viewPortHeight(viewPortHeight)
{	
	createCamera();
	activateCamera(CameraDefault);

	// Initialize View and Projection matrices on startup
	auto& camera = CameraList.at(CameraDefault);
	camera.View = glm::lookAt(
		camera.TransformInfo.cameraPos,
		camera.TransformInfo.cameraPos + camera.TransformInfo.cameraFront,
		camera.TransformInfo.cameraUp
	);
	camera.Projection = glm::perspective(
		glm::radians(camera.SettingsInfo.FOVdegree),
		viewPortWidth / viewPortHeight,
		camera.SettingsInfo.near,
		camera.SettingsInfo.far
	);

	this->counter++;
}

bool CameraManager::hasCamera(long CameraID) {
	auto it = CameraList.find(CameraID);

	if (it == CameraList.end()) {
		return false;
	}
	else {
		return true; 
	}
}

void CameraManager::createCamera() {
	CameraUnit AnotherCamera;
		
	AnotherCamera.CameraID = counter;
	CameraList[AnotherCamera.CameraID] = AnotherCamera;

	this->counter++;
}

void CameraManager::deleteCamera(long CameraID) {
	if (hasCamera(CameraID) && CameraID != CameraDefault) {
		CameraList.erase(CameraID);
		activateCamera(CameraDefault);
	}
	else {
		Logger::addLog(LOG_ERROR, "Failed to delete Camera at index: " + to_string(CameraID));
	}
}

void CameraManager::activateCamera(long CameraID) {
	if (hasCamera(CameraID)) {
		this->activeCameraID = CameraID;
	}
	else {
		Logger::addLog(LOG_ERROR, "Failed to activate Camera at index: " + to_string(CameraID));
	}
}

void CameraManager::changeFOVdegree(long CameraID, float FOVdegree) {
	CameraList.at(CameraID).SettingsInfo.FOVdegree = FOVdegree;
}

void CameraManager::changeNear(long CameraID, float near) {
	CameraList.at(CameraID).SettingsInfo.near = near;
}

void CameraManager::changeFar(long CameraID, float far) {
	CameraList.at(CameraID).SettingsInfo.far = far;
}

void CameraManager::changeSpeed(long CameraID, float TargetSpeed) {
	CameraList.at(CameraID).cameraSpeed = TargetSpeed;
}

void CameraManager::changePos(long CameraID, Movement direction) {

	auto& camera = CameraList.at(CameraID);

	switch (direction) {
	case Forward:
		camera.TransformInfo.cameraPos
			+= camera.cameraSpeed * camera.TransformInfo.cameraFront;
		break;

	case Back:
		camera.TransformInfo.cameraPos
			-= camera.cameraSpeed * camera.TransformInfo.cameraFront;
		break;

	case Right:
		camera.TransformInfo.cameraPos
			+= camera.cameraSpeed * camera.TransformInfo.cameraRight;
		break;

	case Left:
		camera.TransformInfo.cameraPos
			-= camera.cameraSpeed * camera.TransformInfo.cameraRight;
		break;
	}
}

const CameraUnit& CameraManager::getActiveCamera() {
	return CameraList.at(activeCameraID);
}

CameraSettings& CameraManager::getCameraSettings(long CameraID) {
	return CameraList.at(CameraID).SettingsInfo;
}

CameraTransform& CameraManager::getCameraTransform(long CameraID) {
	return CameraList.at(CameraID).TransformInfo;
}

glm::mat4& CameraManager::getCameraView(long CameraID) {
	return CameraList.at(CameraID).View;
}

glm::mat4& CameraManager::getCameraProj(long CameraID) {
	return CameraList.at(CameraID).Projection;
}

const unordered_map <long, CameraUnit>& CameraManager::getCameraList() {
	return CameraList;
}