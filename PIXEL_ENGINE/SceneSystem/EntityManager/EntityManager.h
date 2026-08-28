#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include "../../src/Definitions.h"
using namespace std;


struct EntityUnit {
	unsigned int EntityID;
	MaterialType MaterialID;

	string path;
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

	void createEntity(const string& Path, MaterialType Type, const string& Name = "Object");
	void deactivateEntity(unsigned int EntityID);
	void clearEntityList();
	void Update();
	bool hasEntity(unsigned int EntityID) const;
	bool renameEntity(unsigned int EntityID, const string& Name);
	bool setEntityMaterial(unsigned int EntityID, MaterialType Type);

	const unordered_map <unsigned int, EntityUnit>& getEntityList() const;
	EntityUnit& getEntity(unsigned int EntityID);

};
