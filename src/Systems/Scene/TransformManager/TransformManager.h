#pragma once

#include <glm/glm.hpp>

#include "Core/Definitions/CameraDefinitions.h"
#include "Core/Definitions/EntityDefinitions.h"

class TransformManager {
public:
    void updateEntityMatrix(Transform& transform);

    void updateCameraView(glm::mat4& viewMatrix, CameraTransform& transform);
    void updateCameraProjection(glm::mat4& projectionMatrix, CameraSettings& settings,
                                float viewportWidth, float viewportHeight);

    // Recalculates front/right/up vectors from yaw and pitch
    void updateCameraRotation(CameraTransform& transform, CameraSettings& settings);

private:
    void resetMatrix(glm::mat4& matrix);
    void translate(glm::mat4& matrix, const glm::vec3& translation);
    void rotate(glm::mat4& matrix, const glm::vec3& rotation);
    void scale(glm::mat4& matrix, const glm::vec3& scale);
};