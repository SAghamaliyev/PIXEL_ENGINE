#include "EntityManager.h"

EntityManager::EntityManager() {
	counter = 0;
}

void EntityManager::createEntity(const string& Path, MaterialType Type, const string& Name) {
	EntityUnit OurEntity;
	OurEntity.EntityID = counter;
	OurEntity.path = Path;
	
	if (Name == "Object") {
		OurEntity.name = "Object";
		OurEntity.name += to_string(OurEntity.EntityID);
	}
	else { OurEntity.name = Name; }

	OurEntity.MaterialID = Type;
	OurEntity.isActive = true;

	EntityList[OurEntity.EntityID] = OurEntity;

	++counter;
}

void EntityManager::deactivateEntity(unsigned int EntityID) {
	EntityList[EntityID].isActive = false;
	DeactivatedList.push_back(EntityID);
}

EntityUnit& EntityManager::getEntity(unsigned int EntityID) {
	auto it = EntityList.find(EntityID);
	if (it == EntityList.end()) {
		throw std::out_of_range("Entity with given ID not found");
	}

	return it->second;
}

const unordered_map <unsigned int, EntityUnit>& EntityManager::getEntityList() {
	return EntityList;
}

void EntityManager::Update() {

	for (auto element : DeactivatedList) {
		EntityList.erase(element);
	}
}
