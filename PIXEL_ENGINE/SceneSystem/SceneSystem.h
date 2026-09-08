#pragma once

#include "EntityManager/EntityManager.h"
#include "../src/Definitions.h"

struct SceneInfo {
	const unordered_map <unsigned int, EntityUnit>& EntityList;
};

class SceneSystem {
private:
	EntityManager OurEntityManager;
	SceneInfo OurSceneInfo;
public:
	// System 
	SceneSystem();
	const SceneInfo& getSceneInfo() const;
	void SceneUpdate();

	// EntityManager connection
	void SceneClearEntityList();
    void SceneCreateEntity(unsigned long long int MeshID, MaterialType Type, const string& Name = "Object");
	void SceneDeleteEntity(unsigned int EntityID);
	void SceneDuplicateEntity(unsigned int EntityID);

	void ChangeColorEntity(unsigned int EntityID, Color targetColor);
	void SceneHasEntity(unsigned int EntityID) const;
	void SceneRenameEntity(unsigned int EntityID, const string& Name);
	void SceneSetEntityMaterial(unsigned int EntityID, MaterialType Type);

	EntityUnit& SceneGetEntity(unsigned int EntityID);
};
