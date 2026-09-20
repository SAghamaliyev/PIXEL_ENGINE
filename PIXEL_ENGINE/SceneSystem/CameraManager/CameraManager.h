#pragma once
#include "../../Core/Definitions/SceneDefinitons.h"
#include "../../Core/Logger/Logger.h"
#include <../../glm-1.0.3/glm/glm.hpp>
#include <../../glm-1.0.3/glm/gtc/matrix_transform.hpp>
#include <../../glm-1.0.3/glm/gtc/type_ptr.hpp>

#define CameraDefault 0

class CameraManager {
private:
	unsigned int counter;
	std::unordered_map <unsigned int, CameraUnit> CameraList;
	unsigned int activeCameraID;

	// Window's settings(they are common for every camera)
	float viewPortWidth;
	float viewPortHeight;

	bool hasCamera(unsigned int CameraID);
public:
	CameraManager(float viewPortWidth, float viewPortHeight);

	void createCamera();
	void deleteCamera(unsigned int CameraID);
	void activateCamera(unsigned int CameraID);

	// Setters
	void setViewPort(float viewPortWidth, float viewPortHeight);  // It's Common for all cameras

	void setTransV(unsigned int CameraID, glm::vec3 TransV);

	void setFOVdegree(unsigned int CameraID, float FOVdegree);
	void setNear(unsigned int CameraID, float near);
	void setFar(unsigned int CameraID, float far);

	// Getters
	const CameraUnit& getActiveCamera();
	glm::mat4& getCameraMatrix(unsigned int CameraID);
};