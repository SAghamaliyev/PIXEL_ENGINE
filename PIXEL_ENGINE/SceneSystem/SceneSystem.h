#pragma once

#include "EntityManager/EntityManager.h"
#include "TransformManager/TransformManager.h"
#include "CameraManager/CameraManager.h"
#include "../Core/Definitions/SceneDefinitons.h"

class SceneSystem {
private:
	EntityManager OurEntityManager;
	TransformManager OurTransformManager;
	CameraManager OurCameraManager;

	SceneInfo OurSceneInfo;

	float viewPortWidth;
	float viewPortHeight;
public:
	// SceneSystem 
	SceneSystem(float viewPortWidth, float viewPortHeight);
	const SceneInfo& getSceneInfo() const;
	void SceneUpdate(float viewPortWidth, float viewPortHeight);

	// EntityManager functions
		void SceneClearEntityList();
		void SceneCreateEntity(unsigned long long int MeshID = 2, MaterialType Type = Default, const std::string& Name = "Object");
		void SceneDeleteEntity(long EntityID);
		void SceneDuplicateEntity(long EntityID);
		void MakeActiveEntity(long EntityID);

		// Changers
		void ChangeColorEntity(long EntityID, const Color& targetColor);
		void ChangeNameEntity(long EntityID, const std::string& Name);
		void ChangeMaterialEntity(long EntityID, MaterialType Type);
		void ChangeMeshEntity(long EntityID, uint64_t MeshID);
		void ChangeTextureEntity(long EntityID, uint64_t TetxureID);
		void changeTranslateEntity(long EntityID, const glm::vec3& TranslateV);
		void changeRotateEntity(long EntityID, const glm::vec3& RotateV);
		void changeScaleEntity(long EntityID, const glm::vec3& ScaleV);
			
		// Turn on/off color of Entity
		void activateColorEntity(long EntityID);
		void deactivateColorEntity(long EntityID);
		
		// Getters
		const Transform& getTransformInfoEntity(long EntityID) const;

	// CameraManager functions
		void createSceneCamera();
		void deleteSceneCamera(long CameraID);

		// Setters
		void changeFOVdegreeCamera(long CameraID, float FOVdegree);
		void changeNearCamera(long CameraID, float near);
		void changeFarCamera(long CameraID, float far);
		
		void changeSpeedCamera(long CameraID, float targetSpeed);
		void changePosCamera(long CameraID, Movement direction);
};
