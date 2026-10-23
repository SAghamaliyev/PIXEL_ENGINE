#pragma once

#include <glm/glm.hpp>

struct CameraTransform {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);  // Camera position in world space
    glm::vec3 front    = glm::vec3(0.0f, 0.0f, -1.0f); // Normalized forward direction vector (Z axis)
    glm::vec3 right    = glm::vec3(1.0f, 0.0f, 0.0f);  // Normalized right direction vector (X axis)
    glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f);  // Normalized up direction vector (Y axis)
};

struct CameraSettings {
    float fovDegrees = 45.0f;    // Field of view angle in degrees
    float nearPlane = 0.1f;      // Near clipping plane distance
    float farPlane = 100.0f;     // Far clipping plane distance
    float yaw = -90.0f;          // Yaw angle in degrees (-90.0 points toward negative Z axis)
    float pitch = 0.0f;          // Pitch angle in degrees
    float sensitivity = 0.1f;    // Mouse look sensitivity
    float speed = 0.05f;
};

struct CameraUnit {
    long cameraID;

    glm::mat4 view = glm::mat4(1.0f);        // View matrix (world-to-camera space transform)
    glm::mat4 projection = glm::mat4(1.0f);  // Projection matrix (camera-to-clip space transform)

    CameraTransform transform;
    CameraSettings settings;
};
