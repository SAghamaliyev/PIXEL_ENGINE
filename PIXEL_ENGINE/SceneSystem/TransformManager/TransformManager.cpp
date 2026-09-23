#include "TransformManager.h"

void TransformManager::ResetMatrix(glm::mat4& OurMatrix) {
    OurMatrix = glm::mat4(1.0f);
}

void TransformManager::Translate(glm::mat4& OurMatrix, const glm::vec3& TranslateV) {
	OurMatrix = glm::translate(OurMatrix, TranslateV);
}

void TransformManager::Rotate(glm::mat4& OurMatrix, const glm::vec3& RotateV) {

    // Rotate around X axis
    OurMatrix = glm::rotate(
        OurMatrix,
        glm::radians(RotateV.x),
        glm::vec3(1.0f, 0.0f, 0.0f)
    );

    // Rotate around Y axis
    OurMatrix = glm::rotate(
        OurMatrix,
        glm::radians(RotateV.y),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // Rotate around Z axis
    OurMatrix = glm::rotate(
        OurMatrix,
        glm::radians(RotateV.z),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );
}

void TransformManager::Scale(glm::mat4& OurMatrix, const glm::vec3& ScaleV) {
    OurMatrix = glm::scale(OurMatrix, ScaleV);
}

void TransformManager::TransformEntity(Transform& TransformInfo) {
    ResetMatrix(TransformInfo.OurMatrix);
    Translate(TransformInfo.OurMatrix, TransformInfo.TranslateV);
    Rotate(TransformInfo.OurMatrix, TransformInfo.RotateV);
    Scale(TransformInfo.OurMatrix, TransformInfo.ScaleV);
}

void TransformManager::TransformCameraView(glm::mat4& ViewMatrix, CameraTransform& TransformInfo) {

    ViewMatrix = glm::lookAt(
        TransformInfo.cameraPos,
        TransformInfo.cameraPos + TransformInfo.cameraFront,
        TransformInfo.cameraUp
    );
}

void TransformManager::TransformCameraProj(glm::mat4& ProjectionMatrix, CameraSettings& SettingsInfo,
                                           float viewPortWidth, float viewPortHeight) {

    ProjectionMatrix = glm::perspective(
        glm::radians(SettingsInfo.FOVdegree),
        viewPortWidth / viewPortHeight,
        SettingsInfo.near,
        SettingsInfo.far
    );
}