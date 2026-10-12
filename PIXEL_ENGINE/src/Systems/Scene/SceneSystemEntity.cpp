#include "SceneSystem.h"

#include <stdexcept>
#include <string>

#include "Core/Logger/Logger.h"

using namespace std;

void SceneSystem::sceneClearEntities() {
    m_entityManager.clearEntities();
}

void SceneSystem::sceneCreateEntity(unsigned long long int meshID, 
                                    MaterialType materialType, 
                                    const string& name) {

    m_entityManager.createEntity(meshID, materialType, name);
}


void SceneSystem::sceneDeleteEntity(long entityID) {
    m_entityManager.deactivateEntity(entityID);
}

void SceneSystem::sceneDuplicateEntity(long entityID) {
    m_entityManager.duplicateEntity(entityID);
}

void SceneSystem::sceneSelectEntity(long entityID) {
    m_entityManager.selectEntity(entityID);
}

void SceneSystem::changeColorEntity(long entityID, const Color& color) {
    m_entityManager.changeColor(entityID, color);
}

void SceneSystem::changeNameEntity(long entityID, const string& name) {
    m_entityManager.changeName(entityID, name);
}

void SceneSystem::changeMaterialEntity(long entityID, MaterialType Type) {
    m_entityManager.changeMaterial(entityID, Type);
}

void SceneSystem::changeMeshEntity(long entityID, uint64_t meshID) {
    m_entityManager.changeMesh(entityID, meshID);
}

void SceneSystem::changeTextureEntity(long entityID, uint64_t textureID) {
    m_entityManager.changeTexture(entityID, textureID);
}

void SceneSystem::changeTranslateEntity(long entityID, const glm::vec3& translate) {
    m_entityManager.changeTranslate(entityID, translate);

    auto& tranform_info = m_entityManager.getEntity(entityID).transform;
    m_transformManager.updateEntityMatrix(tranform_info);
}

void SceneSystem::changeRotateEntity(long entityID, const glm::vec3& rotate) {
    m_entityManager.changeRotate(entityID, rotate);

    auto& tranform_info = m_entityManager.getEntity(entityID).transform;
    m_transformManager.updateEntityMatrix(tranform_info);
}

void SceneSystem::changeScaleEntity(long entityID, const glm::vec3& scale) {
    m_entityManager.changeScale(entityID, scale);

    auto& tranform_info = m_entityManager.getEntity(entityID).transform;
    m_transformManager.updateEntityMatrix(tranform_info);
}

void SceneSystem::enableColorTintEntity(long entityID) {
    m_entityManager.enableColorTint(entityID);
}

void SceneSystem::disableColorTintEntity(long entityID) {
    m_entityManager.disableColorTint(entityID);
}

const Transform& SceneSystem::getEntityTransform(long entityID) {
    return m_entityManager.getTransform(entityID);
}
