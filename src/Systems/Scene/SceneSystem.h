#pragma once

#include <cstdint>
#include <string>

#include <glm/glm.hpp>

#include "Core/Definitions/CommonDefinitions.h"
#include "Core/Definitions/MaterialDefinitions.h"
#include "Core/Definitions/SceneDefinitions.h"
#include "Systems/Scene/CameraManager/CameraManager.h"
#include "Systems/Scene/EntityManager/EntityManager.h"
#include "Systems/Scene/TransformManager/TransformManager.h"

class SceneSystem {
public:
    // SceneSystem 
    SceneSystem(int viewportWidth, int viewportHeight);
    const SceneInfo& getSceneInfo();
    void sceneUpdate(int viewportWidth, int viewportHeight);

    // EntityManager functions
        void sceneClearEntities();

        void sceneCreateEntity();
        void sceneCreateEntity(uint64_t  meshID,
                               MaterialType materialType,
                               const std::string& name
        );

        void sceneDeleteEntity(long entityID);
        void sceneDuplicateEntity(long entityID);
        void sceneSelectEntity(long entityID);

        // Entity controls
        void changeColorEntity(long entityID, const Color& color);
        void changeNameEntity(long entityID, const std::string& name);
        void changeMaterialEntity(long entityID, MaterialType materialType);
        void changeMeshEntity(long entityID, uint64_t meshID);
        void changeTextureEntity(long entityID, uint64_t textureID);
        void changeTranslateEntity(long entityID, const glm::vec3& translate);
        void changeRotateEntity(long entityID, const glm::vec3& rotate);
        void changeScaleEntity(long entityID, const glm::vec3& scale);

        void enableColorTintEntity(long entityID);
        void disableColorTintEntity(long entityID);

        // Getters
        const Transform& getEntityTransform(long entityID);

    // CameraManager functions
        void sceneCreateCamera();
        void sceneDeleteCamera(long cameraID);

        // Camera controls
        void changeFovDegreesCamera(long cameraID, float fovDegrees);
        void changeNearPlaneCamera(long cameraID, float nearPlane);
        void changeFarPlaneCamera(long cameraID, float farPlane);

        void changeSpeedCamera(long cameraID, float speed);
        void changePositionCamera(long cameraID, Movement direction);
        void changeRotationCamera(long cameraID, float xOffset, float yOffset);

private:
    EntityManager m_entityManager;
    TransformManager m_transformManager;
    CameraManager m_cameraManager;

    // Holds references to the maps inside the managers above,
    // so it must be declared after them and initialized in the constructor
    SceneInfo m_sceneInfo;

    int m_viewportWidth;
    int m_viewportHeight;
};
