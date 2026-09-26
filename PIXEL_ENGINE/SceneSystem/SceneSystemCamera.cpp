#include "SceneSystem.h"

void SceneSystem::createSceneCamera() {
	OurCameraManager.createCamera();
}

void SceneSystem::deleteSceneCamera(long CameraID) {
	OurCameraManager.deleteCamera(CameraID);
}

void SceneSystem::changeFOVdegreeCamera(long CameraID, float FOVdegree) {

	try{
		OurCameraManager.changeFOVdegree(CameraID, FOVdegree);

		auto& settings_info = OurCameraManager.getCameraSettings(CameraID);
		auto& projection_matrix = OurCameraManager.getCameraProj(CameraID);

		OurTransformManager.TransformCameraProj(
			projection_matrix, 
			settings_info, 
			this->viewPortWidth,
			this->viewPortHeight
		);
	}
	catch (const std::out_of_range& error){
		Logger::addLog(LOG_ERROR, error.what());
	}
}

void SceneSystem::changeNearCamera(long CameraID, float near) {

	try {
		OurCameraManager.changeNear(CameraID, near);

		auto& settings_info = OurCameraManager.getCameraSettings(CameraID);
		auto& projection_matrix = OurCameraManager.getCameraProj(CameraID);

		OurTransformManager.TransformCameraProj(
			projection_matrix,
			settings_info,
			this->viewPortWidth,
			this->viewPortHeight
		);
	}
	catch (const std::out_of_range& error) {
		Logger::addLog(LOG_ERROR, error.what());
	}
}

void SceneSystem::changeFarCamera(long CameraID, float far) {

	try {
		OurCameraManager.changeFar(CameraID, far);

		auto& settings_info = OurCameraManager.getCameraSettings(CameraID);
		auto& projection_matrix = OurCameraManager.getCameraProj(CameraID);

		OurTransformManager.TransformCameraProj(
			projection_matrix,
			settings_info,
			this->viewPortWidth,
			this->viewPortHeight
		);
	}
	catch (const std::out_of_range& error) {
		Logger::addLog(LOG_ERROR, error.what());
	}
}

void SceneSystem::changeSpeedCamera(long CameraID, float targetSpeed) {
	
	try{
		OurCameraManager.changeSpeed(CameraID, targetSpeed);
	}
	catch (const std::out_of_range& error) {
		Logger::addLog(LOG_ERROR, error.what());
	}
}

void SceneSystem::changePosCamera(long CameraID, Movement direction) {

	try {
		OurCameraManager.changePos(CameraID, direction);

		auto& transform_info = OurCameraManager.getCameraTransform(CameraID);
		auto& view_matrix = OurCameraManager.getCameraView(CameraID);

		OurTransformManager.TransformCameraView(view_matrix, transform_info);
	}
	catch (const std::out_of_range& error) {
		Logger::addLog(LOG_ERROR, error.what());
	}
}

void SceneSystem::changeAnglesCamera(long CameraID, float xPos, float yPos) {

	try {
		OurCameraManager.changeAngles(CameraID, xPos, yPos);

		auto& transform_info = OurCameraManager.getCameraTransform(CameraID);
		auto& settings_info = OurCameraManager.getCameraSettings(CameraID);
		auto& view_matrix = OurCameraManager.getCameraView(CameraID);

		OurTransformManager.ChangeCameraMousePos(transform_info, settings_info);
		OurTransformManager.TransformCameraView(view_matrix, transform_info);
	}
	catch (const std::out_of_range& error) {
		Logger::addLog(LOG_ERROR, error.what());
	}
}