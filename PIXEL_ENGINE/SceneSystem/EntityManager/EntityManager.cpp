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
	if (EntityList.find(EntityID) == EntityList.end()) {
		return;
	}

	EntityList[EntityID].isActive = false;
	DeactivatedList.push_back(EntityID);
}

void EntityManager::clearEntityList() {
	EntityList.clear();
	DeactivatedList.clear();
	counter = 0;
}

EntityUnit& EntityManager::getEntity(unsigned int EntityID) {
	auto it = EntityList.find(EntityID);
	if (it == EntityList.end()) {
		throw std::out_of_range("Entity with given ID not found");
	}

	return it->second;
}

const unordered_map <unsigned int, EntityUnit>& EntityManager::getEntityList() const {
	return EntityList;
}

bool EntityManager::hasEntity(unsigned int EntityID) const {
	auto it = EntityList.find(EntityID);
	return it != EntityList.end() && it->second.isActive;
}

bool EntityManager::renameEntity(unsigned int EntityID, const string& Name) {
	auto it = EntityList.find(EntityID);
	if (it == EntityList.end()) {
		return false;
	}

	it->second.name = Name;
	return true;
}

bool EntityManager::setEntityMaterial(unsigned int EntityID, MaterialType Type) {
	auto it = EntityList.find(EntityID);
	if (it == EntityList.end()) {
		return false;
	}

	it->second.MaterialID = Type;
	return true;
}

void EntityManager::Update() {

	for (auto element : DeactivatedList) {
		EntityList.erase(element);
	}

	DeactivatedList.clear();
}
