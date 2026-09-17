#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include "../../src/Definitions.h"
#include "../../Core/Logger/Logger.h"

#define TextureDefault 1	//	1 is default texture for rendering(you can find it in createEntity in .cpp)
#define MeshDefault 2		//	2 is default mesh of cube for rendering 
using namespace std;


struct EntityUnit {
	unsigned int EntityID;
	unsigned int nCopies;	// Numbers of copies of one entity
	uint64_t MeshID;
	uint64_t TextureID;
	MaterialType ShaderID;	// Each Shader is working with Entity's material(for liquid we have liquid shader, for solid solid and etc.)

	Color color;
	string name;
	Transform TransformInfo;

	bool isActive;	// Should we render it or not
	bool isColorActive;	// Should we add color to it or not(defaultly no)
};

class EntityManager {
private:
	unsigned int counter;
	unordered_map <unsigned int, EntityUnit> EntityList;
	vector <unsigned int> DeactivatedList;
public:
	EntityManager();

	// Create initial Entity(it can be just basic cube or we can already give it info)
	void createEntity(uint64_t MeshID = MeshDefault, MaterialType ShaderID = Default,
						const string& Name = "Object");

	// We create Entity which is same(except entity id itself)
	void duplicateEntity(unsigned int EntityID);

	// Delete all List
	void clearEntityList();

	// Check if entity exist and active
	bool hasEntity(unsigned int EntityID);

	// Deactivate Entity(we dont delete it instantly bcs we need to warn everyone firstly)
	void deactivateEntity(unsigned int EntityID);

	// Delete all deactivated Entities
	void Update();

	// Changers
	void changeColor(unsigned int EntityID, const Color& targetColor);
	void changeName(unsigned int EntityID, const string& Name);
	void changeMaterial(unsigned int EntityID, MaterialType ShaderID);
	void changeMesh(unsigned int EntityID, uint64_t MeshID);
	void changeTexture(unsigned int EntityID, uint64_t TetxureID);
	void changeTranslate(unsigned int EntityID, const glm::vec3& TranslateV);
	void changeRotate(unsigned int EntityID, const glm::vec3& RotateV);
	void changeScale(unsigned int EntityID, const glm::vec3& ScaleV);

		// Turn on/off color for Entity
		void deactivateColor(unsigned int EntityID);
		void activateColor(unsigned int EntityID);
	
	// Getters
	//Transform& getTransformInfo(unsigned int EntityID);
	EntityUnit& getEntity(unsigned int EntityID);
	const unordered_map <unsigned int, EntityUnit>& getEntityList() const;
};
