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
	SceneInfo& getSceneInfo();
	void SceneUpdate();

	// EntityManager connection
	void SceneCreateEntity(const string& Path, MaterialType Type, const string& Name = "Object");
	void SceneDeleteEntity(unsigned int EntityID);
	EntityUnit& SceneGetEntity(unsigned int EntityID);
};