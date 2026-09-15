#pragma once

#include "EntityManager/EntityManager.h"
#include "TransformManager/TransformManager.h"

enum MaterialType;	// PreDefined enum

struct SceneInfo {
	const unordered_map <unsigned int, EntityUnit>& EntityList;
};

class SceneSystem {
private:
	EntityManager OurEntityManager;
	TransformManager OurTransformManager;
	SceneInfo OurSceneInfo;
public:
	// SceneSystem 
	SceneSystem();
	const SceneInfo& getSceneInfo() const;
	void SceneUpdate();

	// EntityManager functions
	void SceneClearEntityList();
    void SceneCreateEntity(unsigned long long int MeshID = 2, MaterialType Type = Default, const string& Name = "Object");
	void SceneDeleteEntity(unsigned int EntityID);
	void SceneDuplicateEntity(unsigned int EntityID);

		// Changers
		void ChangeColorEntity(unsigned int EntityID, const Color& targetColor);
		void ChangeNameEntity(unsigned int EntityID, const string& Name);
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
		//Transform& getTransformInfoEntity(unsigned int EntityID);
		/*EntityUnit& SceneGetEntity(unsigned int EntityID);*/
};
