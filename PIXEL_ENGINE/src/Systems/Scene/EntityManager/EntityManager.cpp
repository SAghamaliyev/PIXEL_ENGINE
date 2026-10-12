#include "EntityManager.h"

#include <cstdint>
#include <string>

#include "Core/Logger/Logger.h"

using namespace std;

void EntityManager::createEntity(uint64_t meshID, MaterialType materialType, const string& name) {

    EntityUnit entity;
    entity.entityID = m_counter;
    entity.copyCount = 0;
    entity.meshID = meshID;
    entity.materialType = materialType;
    entity.textureID = DEFAULT_TEXTURE_ID;
    entity.isActive = true;
    entity.isColorTintEnabled = false;

    if (name == "Object") {
        entity.name = "Object" + to_string(entity.entityID);
    }
    else {
        entity.name = name;
    }

    m_entityMap[entity.entityID] = entity;

    ++m_counter;
}

void EntityManager::duplicateEntity(long entityID) {
    if (!hasEntity(entityID)) {
        Logger::addLog(LogType::LOG_ERROR,
            "Failed to find entity and duplicate it: " + to_string(entityID));
        return;
    }

    EntityUnit copy = m_entityMap[entityID];

    copy.entityID = m_counter;
    copy.name = copy.name + "(" + to_string(copy.entityID) + ")";

    m_entityMap[copy.entityID] = copy;

    ++m_counter;
}

void EntityManager::clearEntities() {
    m_entityMap.clear();
    m_deactivatedEntityIDs.clear();
    m_selectedEntityID = NO_ENTITY_ID;
    m_counter = 1;
}

bool EntityManager::hasEntity(long entityID) {
    auto it = m_entityMap.find(entityID);

    if (it != m_entityMap.end() && it->second.isActive) {
        return true;
    }
    return false;
}

void EntityManager::deactivateEntity(long entityID) {
    if (!hasEntity(entityID)) {
        Logger::addLog(LogType::LOG_ERROR, "Entity is either already deactivated or does not exist");
        return;
    }

    m_entityMap[entityID].isActive = false;
    m_deactivatedEntityIDs.push_back(entityID);

    if (m_selectedEntityID == entityID) {
        m_selectedEntityID = NO_ENTITY_ID;
    }
}

void EntityManager::selectEntity(long entityID) {
    if (!hasEntity(entityID)) {
        Logger::addLog(LogType::LOG_ERROR,
            "Failed to find entity and select it: " + to_string(entityID));
        return;
    }

    m_selectedEntityID = entityID;
}

void EntityManager::update() {
    // Clear all deactivated entities
    for (long entity_id : m_deactivatedEntityIDs) {
        m_entityMap.erase(entity_id);
    }

    m_deactivatedEntityIDs.clear();
}

void EntityManager::changeColor(long entityID, const Color& color) {
    if (!hasEntity(entityID)) {
        Logger::addLog(LogType::LOG_ERROR,
            "Failed to find entity and change its Color: " + to_string(entityID));
        return;
    }

    m_entityMap[entityID].color = color;
}

void EntityManager::changeName(long entityID, const string& name) {
    if (!hasEntity(entityID)) {
        Logger::addLog(LogType::LOG_ERROR,
            "Failed to find entity and change its Name: " + to_string(entityID));
        return;
    }

    m_entityMap[entityID].name = name;
}

void EntityManager::changeMaterial(long entityID, MaterialType materialType) {
    if (!hasEntity(entityID)) {
        Logger::addLog(LogType::LOG_ERROR,
            "Failed to find entity and change its Material: " + to_string(entityID));
        return;
    }

    m_entityMap[entityID].materialType = materialType;
}

void EntityManager::changeMesh(long entityID, uint64_t meshID) {
    if (!hasEntity(entityID)) {
        Logger::addLog(LogType::LOG_ERROR,
            "Failed to find entity and change its Mesh: " + to_string(entityID));
        return;
    }

    m_entityMap[entityID].meshID = meshID;
}

void EntityManager::changeTexture(long entityID, uint64_t textureID) {
    if (!hasEntity(entityID)) {
        Logger::addLog(LogType::LOG_ERROR,
            "Failed to find entity and change its Texture: " + to_string(entityID));
        return;
    }

    m_entityMap[entityID].textureID = textureID;
}

void EntityManager::changeTranslate(long entityID, const glm::vec3& translate) {
    if (!hasEntity(entityID)) {
        Logger::addLog(LogType::LOG_ERROR,
            "Failed to find entity and change its Translation: " + to_string(entityID));
        return;
    }

    m_entityMap[entityID].transform.translation = translate;
}

void EntityManager::changeRotate(long entityID, const glm::vec3& rotate) {
    if (!hasEntity(entityID)) {
        Logger::addLog(LogType::LOG_ERROR,
            "Failed to find entity and change its Rotation: " + to_string(entityID));
        return;
    }

    m_entityMap[entityID].transform.rotation = rotate;
}

void EntityManager::changeScale(long entityID, const glm::vec3& scale) {
    if (!hasEntity(entityID)) {
        Logger::addLog(LogType::LOG_ERROR,
            "Failed to find entity and change its Scale: " + to_string(entityID));
        return;
    }

    m_entityMap[entityID].transform.scale = scale;
}

void EntityManager::enableColorTint(long entityID) {
    if (!hasEntity(entityID)) {
        Logger::addLog(LogType::LOG_ERROR,
            "Failed to find entity and enable its Color Tint: " + to_string(entityID));
        return;
    }

    m_entityMap[entityID].isColorTintEnabled = true;
}

void EntityManager::disableColorTint(long entityID) {
    if (!hasEntity(entityID)) {
        Logger::addLog(LogType::LOG_ERROR,
            "Failed to find entity and disable its Color Tint: " + to_string(entityID));
        return;
    }

    m_entityMap[entityID].isColorTintEnabled = false;
}

long EntityManager::getSelectedEntityID() {
    return m_selectedEntityID;
}

EntityUnit& EntityManager::getEntity(long entityID) {
    return m_entityMap.at(entityID);
}

const unordered_map<long, EntityUnit>& EntityManager::getEntities() {
    return m_entityMap;
}

const Transform& EntityManager::getTransform(long entityID) {
    return m_entityMap.at(entityID).transform;
}