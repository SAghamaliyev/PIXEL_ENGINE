#pragma once
#include "../../Core/Definitions/SceneDefinitons.h"
#include "../../Core/Logger/Logger.h"
#include <../../glm-1.0.3/glm/glm.hpp>
#include <../../glm-1.0.3/glm/gtc/matrix_transform.hpp>
#include <../../glm-1.0.3/glm/gtc/type_ptr.hpp>

enum Movement{
	Forward,
	Back,
	Right,
	Left
};

class CameraManager {
private:
	unsigned int counter = 0;
	std::unordered_map <unsigned int, CameraUnit> CameraList;
	unsigned int activeCameraID;
	float viewPortWidth = 800.0f;
	float viewPortHeight = 600.0f;

	bool hasCamera(unsigned int CameraID);
public:
	CameraManager(float viewPortWidth, float viewPortHeight);

	void createCamera();
	void deleteCamera(unsigned int CameraID);
	void activateCamera(unsigned int CameraID);

	// Changers
	void changeFOVdegree(unsigned int CameraID, float FOVdegree);
	void changeNear(unsigned int CameraID, float near);
	void changeFar(unsigned int CameraID, float far);

	void changeSpeed(unsigned int CameraID, float TargetSpeed);
	void changePos(unsigned int CameraID, Movement direction);

	// Getters
	const CameraUnit& getActiveCamera();

	CameraSettings& getCameraSettings(unsigned int CameraID);
	CameraTransform& getCameraTransform(unsigned int CameraID);

	glm::mat4& getCameraView(unsigned int CameraID);
	glm::mat4& getCameraProj(unsigned int CameraID);
};