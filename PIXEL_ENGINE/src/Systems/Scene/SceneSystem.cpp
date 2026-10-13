#include "SceneSystem.h"

SceneSystem::SceneSystem(int viewportWidth, int viewportHeight)
    : m_entityManager()
    , m_transformManager()
    , m_cameraManager(viewportWidth, viewportHeight)
    , m_sceneInfo{
        m_entityManager.getEntities(),
        m_entityManager.getSelectedEntityID(),
        m_cameraManager.getCameraMap(),
        m_cameraManager.getActiveCameraID()
    }
{
    m_viewportWidth = viewportWidth;
    m_viewportHeight = viewportHeight;
}

const SceneInfo& SceneSystem::getSceneInfo() {
    return m_sceneInfo;
}

void SceneSystem::sceneUpdate(int viewportWidth, int viewportHeigh) {
    m_entityManager.update();
}
