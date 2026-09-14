#include "TransformationManager.h"

void TransformationManager::Translate(glm::mat4& OurMatrix, glm::vec3& TranslateV) {
	OurMatrix = glm::translate(OurMatrix, TranslateV);
}

void TransformationManager::Rotate(glm::mat4& OurMatrix, glm::vec3& RotateV) {

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

void TransformationManager::Scale(glm::mat4& OurMatrix, glm::vec3& ScaleV) {
    OurMatrix = glm::scale(OurMatrix, ScaleV);
}