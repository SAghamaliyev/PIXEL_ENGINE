#include "CameraManager.h"

#include <string>

#include <glm/gtc/matrix_transform.hpp>

#include "Core/Logger/Logger.h"
#include "Core/Definitions/CommonDefinitions.h"

using namespace std;

CameraManager::CameraManager(float viewportWidth, float viewportHeight){

    m_viewportWidth = viewportWidth;
    m_viewportHeight = viewportHeight;
    m_activeCameraID = CAMERA_DEFAULT;

    createCamera();
    activateCamera(CAMERA_DEFAULT);
}

bool CameraManager::hasCamera(long cameraID) {
    auto it = m_cameraMap.find(cameraID);

    if (it == m_cameraMap.end()) {
        return false;
    }

    return true;
}

void CameraManager::createCamera() {
    CameraUnit camera;

    camera.cameraID = m_counter;

    camera.view = glm::lookAt(
        camera.transform.position,
        camera.transform.position + camera.transform.front,
        camera.transform.up
    );

    camera.projection = glm::perspective(
        glm::radians(camera.settings.fovDegrees),
        m_viewportWidth / m_viewportHeight,
        camera.settings.nearPlane,
        camera.settings.farPlane
    );

    m_cameraMap[camera.cameraID] = camera;
    m_counter++;
}

void CameraManager::deleteCamera(long cameraID) {
    if (!hasCamera(cameraID) || cameraID == CAMERA_DEFAULT) {
        Logger::addLog(LogType::LOG_ERROR,
                       "Failed to delete camera with ID: " + to_string(cameraID));
        return;
    }

    m_cameraMap.erase(cameraID);
    activateCamera(CAMERA_DEFAULT);
}

void CameraManager::activateCamera(long cameraID) {
    if (!hasCamera(cameraID)) {
        Logger::addLog(LogType::LOG_ERROR,
                       "Failed to activate camera with ID: " + to_string(cameraID));
        return;
    }

    m_activeCameraID = cameraID;
}

void CameraManager::changeFov(long cameraID, float fovDegrees) {
    m_cameraMap.at(cameraID).settings.fovDegrees = fovDegrees;
}

void CameraManager::changeNearPlane(long cameraID, float nearPlane) {
    m_cameraMap.at(cameraID).settings.nearPlane = nearPlane;
}

void CameraManager::changeFarPlane(long cameraID, float farPlane) {
    m_cameraMap.at(cameraID).settings.farPlane = farPlane;
}

void CameraManager::changeSpeed(long cameraID, float speed) {
    m_cameraMap.at(cameraID).settings.speed = speed;
}

void CameraManager::changePosition(long cameraID, Movement direction) {
    auto& camera = m_cameraMap.at(cameraID);

    switch (direction) {
    case Movement::Forward:
        camera.transform.position
            += camera.settings.speed * camera.transform.front;
        break;

    case Movement::Back:
        camera.transform.position
            -= camera.settings.speed * camera.transform.front;
        break;

    case Movement::Right:
        camera.transform.position
            += camera.settings.speed * camera.transform.right;
        break;

    case Movement::Left:
        camera.transform.position
            -= camera.settings.speed * camera.transform.right;
        break;

    case Movement::Up:
        camera.transform.position
            += camera.settings.speed * camera.transform.up;
        break;

    case Movement::Down:
        camera.transform.position
            -= camera.settings.speed * camera.transform.up;
        break;
    }
}

void CameraManager::changeRotation(long cameraID, float xOffset, float yOffset) {
    auto& settings = m_cameraMap.at(cameraID).settings;

    xOffset *= settings.sensitivity;
    yOffset *= settings.sensitivity;

    settings.yaw += xOffset;
    settings.pitch += yOffset;

    if (settings.pitch > 89.0f) {
        settings.pitch = 89.0f;
    }

    if (settings.pitch < -89.0f) {
        settings.pitch = -89.0f;
    }
}

long CameraManager::getActiveCameraID() {
    return m_activeCameraID;
}

const unordered_map<long, CameraUnit>& CameraManager::getCameraMap() {
    return m_cameraMap;
}

CameraSettings& CameraManager::getSettings(long cameraID) {
    return m_cameraMap.at(cameraID).settings;
}

CameraTransform& CameraManager::getTransform(long cameraID) {
    return m_cameraMap.at(cameraID).transform;
}

glm::mat4& CameraManager::getView(long cameraID) {
    return m_cameraMap.at(cameraID).view;
}

glm::mat4& CameraManager::getProjection(long cameraID) {
    return m_cameraMap.at(cameraID).projection;
}