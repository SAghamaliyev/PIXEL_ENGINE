#pragma once
#include "../../Core/Definitions/SceneDefinitons.h"
#include "../../Core/Logger/Logger.h"

#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>

#define TextureDefault 1 //	1 is default texture for rendering(you can find it in createEntity in .cpp)
#define MeshDefault 2	 //	2 is default mesh of cube for rendering 

class EntityManager {
private:
	long counter = 1;
	std::unordered_map <long, EntityUnit> EntityList;
	long OurActiveEntityID = -1;

	std::vector <long> DeactivatedList;
public:
	EntityManager();

	// Create initial Entity(it can be just basic cube or we can already give it info)
	void createEntity(uint64_t MeshID = MeshDefault, MaterialType ShaderID = Default,
					const std::string& Name = "Object");

	// We create Entity which is same(except entity id itself)
	void duplicateEntity(long EntityID);

	// Delete all List
	void clearEntityList();

	// Check if entity exist and active
	bool hasEntity(long EntityID);

	// Deactivate Entity(we dont delete it instantly bcs we need to warn everyone firstly)
	void deactivateEntity(long EntityID);

	// We select defined entity
	void makeActive(long EntityID);

	// Delete all deactivated Entities
	void Update();

	// Changers
	void changeColor(long EntityID, const Color& targetColor);
	void changeName(long EntityID, const std::string& Name);
	void changeMaterial(long EntityID, MaterialType ShaderID);
	void changeMesh(long EntityID, uint64_t MeshID);
	void changeTexture(long EntityID, uint64_t TetxureID);
	void changeTranslate(long EntityID, const glm::vec3& TranslateV);
	void changeRotate(long EntityID, const glm::vec3& RotateV);
	void changeScale(long EntityID, const glm::vec3& ScaleV);

	// Turn on/off color for Entity
	void deactivateColor(long EntityID);
	void activateColor(long EntityID);
	
	// Getters
	long& getActiveEntityID();
	EntityUnit& getEntity(long EntityID);
	const std::unordered_map <long, EntityUnit>& getEntityList() const;
	const Transform& getTransformInfo(long EntityID) const;
};
