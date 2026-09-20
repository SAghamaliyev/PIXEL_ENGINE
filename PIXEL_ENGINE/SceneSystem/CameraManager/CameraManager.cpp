#include "CameraManager.h"

#include <string>
using namespace std;

CameraManager::CameraManager(float viewPortWidth, float viewPortHeight) {
	CameraUnit OurDefaultCamera;

	this->counter = 1;

	this->viewPortWidth = viewPortWidth;
	this->viewPortHeight = viewPortHeight;

	OurDefaultCamera.CameraID = CameraDefault;
	OurDefaultCamera.TransV = glm::vec3(0.0f, 0.0f, 0.0f);
	OurDefaultCamera.FOVdegree = 45.0f;
	OurDefaultCamera.near = 0.1f;
	OurDefaultCamera.far = 100.0f;
	OurDefaultCamera.View = glm::mat4(1.0f);
	OurDefaultCamera.Projection = glm::perspective(glm::radians(OurDefaultCamera.FOVdegree),
		viewPortWidth / viewPortHeight, OurDefaultCamera.near, OurDefaultCamera.far);
	OurDefaultCamera.OurMatrix = glm::mat4(1.0f);

	CameraList[CameraDefault] = OurDefaultCamera;
	this->activeCameraID = CameraDefault;
}

bool CameraManager::hasCamera(unsigned int CameraID) {
	auto it = CameraList.find(CameraID);

	if (it == CameraList.end()) {
		return false;
	}
	else { return true; }
}

void CameraManager::createCamera() {
	CameraUnit AnotherCamera;
		
	AnotherCamera.CameraID = counter;
	AnotherCamera.TransV = glm::vec3(0.0f, 0.0f, 0.0f);
	AnotherCamera.FOVdegree = 45.0f;
	AnotherCamera.near = 0.1f;
	AnotherCamera.far = 100.0f;
	AnotherCamera.View = glm::mat4(1.0f);
	AnotherCamera.Projection = glm::perspective(glm::radians(AnotherCamera.FOVdegree),
		this->viewPortWidth / this->viewPortHeight, AnotherCamera.near, AnotherCamera.far);
	AnotherCamera.OurMatrix = glm::mat4(1.0f);

	CameraList[AnotherCamera.CameraID] = AnotherCamera;
}

void CameraManager::deleteCamera(unsigned int CameraID) {
	if (hasCamera(CameraID) && CameraID != CameraDefault) {
		CameraList.erase(CameraID);
		activateCamera(CameraDefault);
	}
	else {
		Logger::addLog(LOG_ERROR, "Failed to delete Camera at index: " + to_string(CameraID));
	}
}

void CameraManager::activateCamera(unsigned int CameraID) {
	if (hasCamera(CameraID)) {
		this->activeCameraID = CameraID;
	}
	else {
		Logger::addLog(LOG_ERROR, "Failed to activate Camera at index: " + to_string(CameraID));
	}
}

void CameraManager::setViewPort(float viewPortWidth, float viewPortHeight) {
	this->viewPortWidth = viewPortWidth;
	this->viewPortHeight = viewPortHeight;
}

void CameraManager::setTransV(unsigned int CameraID, glm::vec3 TransV) {
	if (hasCamera(CameraID)) {
		CameraList[CameraID].TransV = TransV;
	}
	else {
		Logger::addLog(LOG_ERROR, "Failed to find and change TransV Camera at index: " + to_string(CameraID));
	}
}

void CameraManager::setFOVdegree(unsigned int CameraID, float FOVdegree) {
	if (hasCamera(CameraID)) {
		CameraList[CameraID].FOVdegree = FOVdegree;
	}
	else {
		Logger::addLog(LOG_ERROR, "Failed to find and change FOVdegree Camera at index: " + to_string(CameraID));
	}
}

void CameraManager::setNear(unsigned int CameraID, float near) {
	if (hasCamera(CameraID)) {
		CameraList[CameraID].near = near;
	}
	else {
		Logger::addLog(LOG_ERROR, "Failed to find and change near Camera at index: " + to_string(CameraID));
	}
}

void CameraManager::setFar(unsigned int CameraID, float far) {
	if (hasCamera(CameraID)) {
		CameraList[CameraID].far = far;
	}
	else {
		Logger::addLog(LOG_ERROR, "Failed to find and change far Camera at index: " + to_string(CameraID));
	}
}

const CameraUnit& CameraManager::getActiveCamera() {
	return CameraList.at(activeCameraID);
}

glm::mat4& CameraManager::getCameraMatrix(unsigned int CameraID) {
	return CameraList.at(CameraID).OurMatrix;
}