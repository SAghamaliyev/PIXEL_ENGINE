#pragma once
#include "../../Core/Definitions/SceneDefinitons.h"

class TransformManager {
private:
	void ResetMatrix(glm::mat4& OurMatrix);
	void Translate(glm::mat4& OurMatrix, const glm::vec3& TranslateV);
	void Rotate(glm::mat4& OurMatrix, const glm::vec3& RotateV);
	void Scale(glm::mat4& OurMatrix, const glm::vec3& ScaleV);
public:
	void TransformEntity(Transform& TransformInfo);

	void TransformCameraView(glm::mat4& ViewMatrix, CameraTransform& TransformInfo);
	void TransformCameraProj(glm::mat4& ProjectionMatrix, CameraSettings& SettingsInfo, 
							 float viewPortWidth, float viewPortHeight);

	void ChangeCameraMousePos(CameraTransform& TransformInfo, CameraSettings& SettingsInfo);
};