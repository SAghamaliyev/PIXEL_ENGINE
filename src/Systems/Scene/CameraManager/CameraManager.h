#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

#include "Core/Definitions/CameraDefinitions.h"
#include "Core/Definitions/CommonDefinitions.h"

class CameraManager {
public:
    CameraManager(float viewPortWidth, float viewPortHeight);

    void createCamera();
    void deleteCamera(long cameraID);
    void activateCamera(long cameraID);

    // Camera controls
    void changeFov(long cameraID, float fovDegrees);
    void changeNearPlane(long cameraID, float nearPlane);
    void changeFarPlane(long cameraID, float farPlane);

    void changeSpeed(long cameraID, float speed);
    void changePosition(long cameraID, Movement direction);
    void changeRotation(long cameraID, float xOffset, float yOffset);  // Change yaw and pitch angles of camera 

    // Getters
    long getActiveCameraID();
    const std::unordered_map <long, CameraUnit>& getCameraMap();

    // These 4 getters cannot be constant, because they are used in Transformsystem and can be modified
    CameraSettings& getSettings(long cameraID);
    CameraTransform& getTransform(long cameraID);
    glm::mat4& getView(long cameraID);
    glm::mat4& getProjection(long cameraID);

private:
    long m_counter = 1;

    std::unordered_map <long, CameraUnit> m_cameraMap;
    long m_activeCameraID;

    float m_viewportWidth;
    float m_viewportHeight;

    bool hasCamera(long cameraID);
};