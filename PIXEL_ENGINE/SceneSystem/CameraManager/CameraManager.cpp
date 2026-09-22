#include "CameraManager.h"
#include <string>

#define CameraDefault 0
using namespace std;

CameraManager::CameraManager(float viewPortWidth, float viewPortHeight) {	
	createCamera();
	activateCamera(CameraDefault);

	this->counter++;
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
	CameraList[AnotherCamera.CameraID] = AnotherCamera;

	this->counter++;
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

void CameraManager::changeFOVdegree(unsigned int CameraID, float FOVdegree) {
	if (hasCamera(CameraID)) {
		CameraList[CameraID].FOVdegree = FOVdegree;
	}
	else {
		Logger::addLog(LOG_ERROR, "Failed to find and change FOVdegree Camera at index: " + to_string(CameraID));
	}
}

void CameraManager::changeNear(unsigned int CameraID, float near) {
	if (hasCamera(CameraID)) {
		CameraList[CameraID].near = near;
	}
	else {
		Logger::addLog(LOG_ERROR, "Failed to find and change near Camera at index: " + to_string(CameraID));
	}
}

void CameraManager::changeFar(unsigned int CameraID, float far) {
	if (hasCamera(CameraID)) {
		CameraList[CameraID].far = far;
	}
	else {
		Logger::addLog(LOG_ERROR, "Failed to find and change far Camera at index: " + to_string(CameraID));
	}
}

void CameraManager::changeSpeed(unsigned int CameraID, float TargetSpeed) {
	if (hasCamera(CameraID)) {
		CameraList[CameraID].cameraSpeed = TargetSpeed;
	}
	else {
		Logger::addLog(LOG_ERROR, "Failed to find and change speed Camera at index: " + to_string(CameraID));
	}
}

void CameraManager::changePos(unsigned int CameraID, Movement direction) {
	if (hasCamera(CameraID)) {
		switch (direction)
		{
		case Forward:
			CameraList[CameraID].cameraPos += CameraList[CameraID].cameraSpeed * CameraList[CameraID].cameraFront;
			break;
		case Back:
			CameraList[CameraID].cameraPos -= CameraList[CameraID].cameraSpeed * CameraList[CameraID].cameraFront;
			break;
		case Right:
			CameraList[CameraID].cameraPos += CameraList[CameraID].cameraSpeed * CameraList[CameraID].cameraRight;
			break;
		case Left:
			CameraList[CameraID].cameraPos -= CameraList[CameraID].cameraSpeed * CameraList[CameraID].cameraRight;
			break;
		}
	}
	else {
		Logger::addLog(LOG_ERROR, "Failed to find and change position Camera at index: " + to_string(CameraID));
	}
}

const CameraUnit& CameraManager::getActiveCamera() {
	return CameraList.at(activeCameraID);
}