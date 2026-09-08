#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include "../../src/Definitions.h"
#include "../../Logger/Logger.h"
using namespace std;


struct EntityUnit {
	unsigned int EntityID;	
	unsigned long long int MeshID;
	MaterialType MaterialID;	//ShaderID
	Color color;

	string name;
	bool isActive;
};

class EntityManager {
private:
	unsigned int counter;
	unordered_map <unsigned int, EntityUnit> EntityList;
	vector <unsigned int> DeactivatedList;
public:
	EntityManager();

	// Create initial Entity(it can be just basic cube or we can already give it info)
	void createEntity(unsigned long long int MeshID = 123, MaterialType Type = Default,
						const string& Name = "Object");

	// Deactivate Entity(we dont delete it instantly bcs we need to warn everyone firstly)
	void deactivateEntity(unsigned int EntityID);

	// We create Entity which is same(except entity id itself)
	void duplicateEntity(unsigned int EntityID);

	// Delete all List
	void clearEntityList();

	// Delete all deactivated Entities
	void Update();

	bool hasEntity(unsigned int EntityID) const;

	// Changers
	void changeColor(unsigned int EntityID, Color targetColor);
	void changeName(unsigned int EntityID, const string& Name);
	void changeMaterial(unsigned int EntityID, MaterialType Type);
	void changeMesh(unsigned int EntityID, unsigned long long int MeshID);

	const unordered_map <unsigned int, EntityUnit>& getEntityList() const;
	EntityUnit& getEntity(unsigned int EntityID);

};
