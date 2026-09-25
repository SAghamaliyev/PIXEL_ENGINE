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
	long counter = 0;
	std::unordered_map <long, CameraUnit> CameraList;
	long activeCameraID;
	float viewPortWidth = 800.0f;
	float viewPortHeight = 600.0f;

	bool hasCamera(long CameraID);
public:
	CameraManager(float viewPortWidth, float viewPortHeight);

	void createCamera();
	void deleteCamera(long CameraID);
	void activateCamera(long CameraID);

	// Changers
	void changeFOVdegree(long CameraID, float FOVdegree);
	void changeNear(long CameraID, float near);
	void changeFar(long CameraID, float far);

	void changeSpeed(long CameraID, float TargetSpeed);
	void changePos(long CameraID, Movement direction);

	// Getters
	const CameraUnit& getActiveCamera();

	CameraSettings& getCameraSettings(long CameraID);
	CameraTransform& getCameraTransform(long CameraID);

	glm::mat4& getCameraView(long CameraID);
	glm::mat4& getCameraProj(long CameraID);

	const std::unordered_map <long, CameraUnit>& getCameraList();
};