#include "EntityManager.h"

#include <filesystem>

EntityManager::EntityManager() {
	counter = 0;
}

void EntityManager::createEntity(unsigned long long int MeshID, MaterialType Type,
								const string& Name) {

	EntityUnit OurEntity;
	OurEntity.EntityID = counter;
	OurEntity.MeshID = MeshID;
	OurEntity.MaterialID = Type;
	OurEntity.isActive = true;
	
	
	if (Name == "Object") {
		OurEntity.name = "Object";
		OurEntity.name += to_string(OurEntity.EntityID);
	}
	else { OurEntity.name = Name; }


	EntityList[OurEntity.EntityID] = OurEntity;

	++counter;
}

void EntityManager::duplicateEntity(unsigned int EntityID) {
	EntityUnit r = EntityList[EntityID];
	createEntity(r.MeshID, r.MaterialID, r.name + "dup");
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

		Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
			"Entity with given ID not found");
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

void EntityManager::changeColor(unsigned int EntityID, Color targetColor) {
	auto it = EntityList.find(EntityID);
	if (it == EntityList.end()) {
		Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR, 
			"Failed to find entity: " + EntityList[EntityID].name);
		return;
	}

	EntityList[EntityID].color = targetColor;
}

void EntityManager::changeName(unsigned int EntityID, const string& Name) {
	auto it = EntityList.find(EntityID);
	if (it == EntityList.end()) {
		Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
			"Failed to find entity: " + EntityList[EntityID].name);
		return;
	}

	it->second.name = Name;
}

void EntityManager::changeMaterial(unsigned int EntityID, MaterialType Type) {
	auto it = EntityList.find(EntityID);
	if (it == EntityList.end()) {
		Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
			"Failed to find entity: " + EntityList[EntityID].name);
		return;
	}

	it->second.MaterialID = Type;
}

void EntityManager::changeMesh(unsigned int EntityID, unsigned long long int MeshID) {
	auto it = EntityList.find(EntityID);
	if (it == EntityList.end()) {
		Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
			"Failed to find entity: " + EntityList[EntityID].name);
		return;
	}

	it->second.MeshID = MeshID;
}

void EntityManager::Update() {

	for (auto element : DeactivatedList) {
		EntityList.erase(element);
	}

	DeactivatedList.clear();
}
