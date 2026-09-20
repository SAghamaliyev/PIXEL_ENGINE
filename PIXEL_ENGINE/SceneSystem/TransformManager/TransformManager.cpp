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

void TransformManager::ReTransformEntity(Transform& TransformInfo) {
    ResetMatrix(TransformInfo.OurMatrix);
    Translate(TransformInfo.OurMatrix, TransformInfo.TranslateV);
    Rotate(TransformInfo.OurMatrix, TransformInfo.RotateV);
    Scale(TransformInfo.OurMatrix, TransformInfo.ScaleV);
}

void TransformManager::ReTransformCamera(glm::mat4& OurMatrix, glm::vec3& TransV) {
    Translate(OurMatrix, TransV);
}