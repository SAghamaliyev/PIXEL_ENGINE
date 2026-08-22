#include "EntityManager/EntityManager.h"

struct SceneInfo {
	const unordered_map <unsigned int, EntityUnit>& EntityList;
};

class SceneSystem {
private:
	EntityManager OurEntityManager;
	SceneInfo OurSceneInfo;
public:
	SceneSystem();
	SceneInfo& getSceneInfo();

	void SceneCreateEntity(const string& Path, MaterialType Type, const string& Name = "Object");
	void SceneDeleteEntity(unsigned int EntityID);
	EntityUnit& SceneGetEntity(unsigned int EntityID);

	void SceneUpdate();
};