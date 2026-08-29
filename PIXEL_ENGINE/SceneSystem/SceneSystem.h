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
	void SceneCreateEntity(const string& Path, MaterialType Type, const string& Name = "Object");
	//void SceneCreateEntity(MaterialType Type, const string& Name = "Object");
	void SceneDeleteEntity(unsigned int EntityID);

	bool SceneHasEntity(unsigned int EntityID) const;
	bool SceneRenameEntity(unsigned int EntityID, const string& Name);
	bool SceneSetEntityMaterial(unsigned int EntityID, MaterialType Type);

	EntityUnit& SceneGetEntity(unsigned int EntityID);
};
