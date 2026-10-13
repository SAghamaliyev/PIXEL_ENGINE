#include "SceneSystem.h"

#include <stdexcept>

#include "Core/Logger/Logger.h"

using namespace std;

void SceneSystem::sceneCreateCamera() {
    m_cameraManager.createCamera();
}

void SceneSystem::sceneDeleteCamera(long cameraID) {
    m_cameraManager.deleteCamera(cameraID);
}

void SceneSystem::changeFovDegreesCamera(long cameraID, float fovDegrees) {
    try {
        m_cameraManager.changeFov(cameraID, fovDegrees);

        auto& settings_info = m_cameraManager.getSettings(cameraID);
        auto& projection_matrix = m_cameraManager.getProjection(cameraID);

        m_transformManager.updateCameraProjection(projection_matrix,
                                                  settings_info,
                                                  m_viewportWidth,
                                                  m_viewportHeight);
    }
    catch (const out_of_range& error) {
        Logger::addLog(LogType::LOG_ERROR, error.what());
    }
}

void SceneSystem::changeNearPlaneCamera(long cameraID, float nearPlane) {
    try {
        m_cameraManager.changeNearPlane(cameraID, nearPlane);

        auto& settings_info = m_cameraManager.getSettings(cameraID);
        auto& projection_matrix = m_cameraManager.getProjection(cameraID);

        m_transformManager.updateCameraProjection(projection_matrix,
                                                  settings_info,
                                                  m_viewportWidth,
                                                  m_viewportHeight);
    }
    catch (const std::out_of_range& error) {
        Logger::addLog(LogType::LOG_ERROR, error.what());
    }
}

void SceneSystem::changeFarPlaneCamera(long cameraID, float farPlane) {
    try {
        m_cameraManager.changeFarPlane(cameraID, farPlane);

        auto& settings_info = m_cameraManager.getSettings(cameraID);
        auto& projection_matrix = m_cameraManager.getProjection(cameraID);

        m_transformManager.updateCameraProjection(projection_matrix,
                                                  settings_info,
                                                  m_viewportWidth,
                                                  m_viewportHeight);
    }
    catch (const std::out_of_range& error) {
        Logger::addLog(LogType::LOG_ERROR, error.what());
    }
}

void SceneSystem::changeSpeedCamera(long cameraID, float speed) {
    try {
        m_cameraManager.changeSpeed(cameraID, speed);
    }
    catch (const std::out_of_range& error) {
        Logger::addLog(LogType::LOG_ERROR, error.what());
    }
}

void SceneSystem::changePositionCamera(long cameraID, Movement direction) {
    try {
        m_cameraManager.changePosition(cameraID, direction);

        auto& transform_info = m_cameraManager.getTransform(cameraID);
        auto& view_matrix = m_cameraManager.getView(cameraID);

        m_transformManager.updateCameraView(view_matrix, transform_info);
    }
    catch (const std::out_of_range& error) {
        Logger::addLog(LogType::LOG_ERROR, error.what());
    }
}

void SceneSystem::changeRotationCamera(long cameraID, float xOffset, float yOffset) {
    try {
        m_cameraManager.changeRotation(cameraID, xOffset, yOffset);

        auto& transform_info = m_cameraManager.getTransform(cameraID);
        auto& settings_info = m_cameraManager.getSettings(cameraID);
        auto& view_matrix = m_cameraManager.getView(cameraID);

        m_transformManager.updateCameraRotation(transform_info, settings_info);
        m_transformManager.updateCameraView(view_matrix, transform_info);
    }
    catch (const std::out_of_range& error) {
        Logger::addLog(LogType::LOG_ERROR, error.what());
    }
}