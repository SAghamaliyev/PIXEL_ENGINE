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
public:
	// SceneSystem 
	SceneSystem(float viewPortWidth, float viewPortHeight);
	const SceneInfo& getSceneInfo() const;
	void SceneUpdate();

	// EntityManager functions
		void SceneClearEntityList();
		void SceneCreateEntity(unsigned long long int MeshID = 2, MaterialType Type = Default, const std::string& Name = "Object");
		void SceneDeleteEntity(unsigned int EntityID);
		void SceneDuplicateEntity(unsigned int EntityID);

		// Changers
		void ChangeColorEntity(unsigned int EntityID, const Color& targetColor);
		void ChangeNameEntity(unsigned int EntityID, const std::string& Name);
		void ChangeMaterialEntity(unsigned int EntityID, MaterialType Type);
		void ChangeMeshEntity(unsigned int EntityID, uint64_t MeshID);
		void ChangeTextureEntity(unsigned int EntityID, uint64_t TetxureID);
		void changeTranslateEntity(unsigned int EntityID, const glm::vec3& TranslateV);
		void changeRotateEntity(unsigned int EntityID, const glm::vec3& RotateV);
		void changeScaleEntity(unsigned int EntityID, const glm::vec3& ScaleV);
			
		// Turn on/off color of Entity
		void activateColorEntity(unsigned int EntityID);
		void deactivateColorEntity(unsigned int EntityID);
		
		// Getters
		const Transform& getTransformInfoEntity(unsigned int EntityID) const;
		/*EntityUnit& SceneGetEntity(unsigned int EntityID);*/

	// CameraManager functions
		void createSceneCamera();
		void deleteSceneCamera(unsigned int CameraID);

		// Setters
		void setSceneViewPort(float viewPortWidth, float viewPortHeight);  // It's Common for all cameras

		void changeTranslateCamera(unsigned int CameraID, glm::vec3 TransV);

		void setFOVdegreeCamera(unsigned int CameraID, float FOVdegree);
		void setNearCamera(unsigned int CameraID, float near);
		void setFarCamera(unsigned int CameraID, float far);
};
