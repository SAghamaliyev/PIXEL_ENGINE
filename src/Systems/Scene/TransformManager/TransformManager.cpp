#include "TransformManager.h"

#include <glm/gtc/matrix_transform.hpp>

void TransformManager::resetMatrix(glm::mat4& matrix) {
    matrix = glm::mat4(1.0f);
}

void TransformManager::translate(glm::mat4& matrix, const glm::vec3& translation) {
    matrix = glm::translate(matrix, translation);
}

void TransformManager::rotate(glm::mat4& matrix, const glm::vec3& rotation) {
    // Rotate around X axis
    matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

    // Rotate around Y axis
    matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

    // Rotate around Z axis
    matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void TransformManager::scale(glm::mat4& matrix, const glm::vec3& scale) {
    matrix = glm::scale(matrix, scale);
}

void TransformManager::updateEntityMatrix(Transform& transform) {
    resetMatrix(transform.matrix);
    translate(transform.matrix, transform.translation);
    rotate(transform.matrix, transform.rotation);
    scale(transform.matrix, transform.scale);
}

void TransformManager::updateCameraView(glm::mat4& viewMatrix, CameraTransform& transform) {
    viewMatrix = glm::lookAt(transform.position,
                             transform.position + transform.front,
                             transform.up);
}

void TransformManager::updateCameraProjection(glm::mat4& projectionMatrix, CameraSettings& settings,
    float viewportWidth, float viewportHeight) {
    projectionMatrix = glm::perspective(glm::radians(settings.fovDegrees),
                                        viewportWidth / viewportHeight,
                                        settings.nearPlane,
                                        settings.farPlane);
}

void TransformManager::updateCameraRotation(CameraTransform& transform, CameraSettings& settings) {
    float yaw_radians = glm::radians(settings.yaw);
    float pitch_radians = glm::radians(settings.pitch);

    glm::vec3 direction;
    direction.x = glm::cos(yaw_radians) * glm::cos(pitch_radians);
    direction.y = glm::sin(pitch_radians);
    direction.z = glm::sin(yaw_radians) * glm::cos(pitch_radians);

    transform.front = glm::normalize(direction);
    transform.right = glm::normalize(glm::cross(transform.front, glm::vec3(0.0f, 1.0f, 0.0f)));
    transform.up = glm::normalize(glm::cross(transform.right, transform.front));
}