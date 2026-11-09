#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "Core/Definitions/EntityDefinitions.h"
#include "Core/Definitions/MaterialDefinitions.h"
#include "Core/Definitions/CommonDefinitions.h"

class EntityManager {
public:
    void createEntity();

    void createEntity(uint64_t meshID,
                      MaterialType materialType,
                      const std::string& name);

    // Only the ID differs from the original entity
    void duplicateEntity(long entityID);

    // Delete all entity map
    void clearEntities();

    bool hasEntity(long entityID);

    // Deactivate the entity (we don't delete it immediately, as other modules must be notified first)
    void deactivateEntity(long entityID);

    void selectEntity(long entityID);

    // Delete all deactivated entities
    void update();

    // Entity controls
    void changeColor(long entityID, const Color& color);
    void changeName(long entityID, const std::string& name);
    void changeMaterial(long entityID, MaterialType shaderID);
    void changeMesh(long entityID, uint64_t meshID);
    void changeTexture(long entityID, uint64_t textureID);
    void changeTranslate(long entityID, const glm::vec3& translate);
    void changeRotate(long entityID, const glm::vec3& rotate);
    void changeScale(long entityID, const glm::vec3& scale);

    void enableColorTint(long entityID);
    void disableColorTint(long entityID);

    // Getters
    long getSelectedEntityID();

    // This getter cannot be const, since we use it in SceneSystem/TransformManager and changes it
    EntityUnit& getEntity(long entityID);

    const std::unordered_map <long, EntityUnit>& getEntities();
    const Transform& getTransform(long entityID);

private:
    long m_counter = 1;
    std::unordered_map <long, EntityUnit> m_entityMap;

    // Selected entity is the entity that the user chose by clicking on it
    long m_selectedEntityID = NO_ENTITY_ID;

    std::vector <long> m_deactivatedEntityIDs;
};
