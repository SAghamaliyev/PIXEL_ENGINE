#include "EntityManager.h"

#include <filesystem>

EntityManager::EntityManager() {
	counter = 0;
}

void EntityManager::createEntity(uint64_t MeshID, MaterialType Type,
								const string& Name) {

	EntityUnit OurEntity;
	OurEntity.EntityID = counter;
	OurEntity.MeshID = MeshID;
	OurEntity.MaterialID = Type;
	OurEntity.isActive = true;
	OurEntity.TextureID = 1;
	OurEntity.isColorActive = false;
	
	
	if (Name == "Object") {
		OurEntity.name = "Object";
		OurEntity.name += to_string(OurEntity.EntityID);
	}
	else { OurEntity.name = Name; }


	EntityList[OurEntity.EntityID] = OurEntity;

	++counter;
}

void EntityManager::duplicateEntity(unsigned int EntityID) {
	EntityUnit temp = EntityList[EntityID];
	
	temp.EntityID = counter;
	temp.name = temp.name + "(" + to_string(temp.EntityID) + ")";

	EntityList[temp.EntityID] = temp;

	++counter;
}

void EntityManager::clearEntityList() {
	EntityList.clear();
	DeactivatedList.clear();
	counter = 0;
}

bool EntityManager::hasEntity(unsigned int EntityID) {

	auto it = EntityList.find(EntityID);

	if (it == EntityList.end() && it->second.isActive)
		return false;
	return true;
}

void EntityManager::deactivateEntity(unsigned int EntityID) {

	if (hasEntity(EntityID)) {
		EntityList[EntityID].isActive = false;
		DeactivatedList.push_back(EntityID);
	}
	else {
		Logger::addLog(LOG_ERROR, "Entity is either already deactivated or not exist");
		return;
	}
}

void EntityManager::Update() {

	for (auto element : DeactivatedList) {
		EntityList.erase(element);
	}

	DeactivatedList.clear();
}

void EntityManager::changeColor(unsigned int EntityID, Color targetColor) {

	if (hasEntity(EntityID)) {
		EntityList[EntityID].color = targetColor;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Color: " + EntityList[EntityID].name);
		return;
	}
}

void EntityManager::changeName(unsigned int EntityID, const string& Name) {

	if (hasEntity(EntityID)) {
		EntityList[EntityID].name = Name;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Name: " + EntityList[EntityID].name);
		return;
	}
}

void EntityManager::changeMaterial(unsigned int EntityID, MaterialType Type) {

	if (hasEntity(EntityID)) {
		EntityList[EntityID].MaterialID = Type;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Material: " + EntityList[EntityID].name);
		return;
	}
}

void EntityManager::changeMesh(unsigned int EntityID, uint64_t MeshID) {

	if (hasEntity(EntityID)) {
		EntityList[EntityID].MeshID = MeshID;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Mesh: " + EntityList[EntityID].name);
		return;
	}
}

void EntityManager::changeTexture(unsigned int EntityID, uint64_t TextureID) {

	if (hasEntity(EntityID)) {
		EntityList[EntityID].TextureID = TextureID;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Texture: " + std::to_string(EntityID));
		return;
	}
}

void EntityManager::activateColor(unsigned int EntityID) {

	if (hasEntity(EntityID)) {
		EntityList[EntityID].isColorActive = true;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and activate its Color: " + std::to_string(EntityID));
		return;
	}
}

void EntityManager::deactivateColor(unsigned int EntityID) {

	if (hasEntity(EntityID)) {
		EntityList[EntityID].isColorActive = false;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and deactivate its Color: " + std::to_string(EntityID));
		return;
	}
}

const unordered_map <unsigned int, EntityUnit>& EntityManager::getEntityList() const {
	return EntityList;
}

//EntityUnit& EntityManager::getEntity(unsigned int EntityID) {
//
//	if (hasEntity(EntityID)) {
//		return EntityList[EntityID];
//	}
//	else {
//		Logger::addLog(LOG_ERROR, "Entity with given ID not found");
//		
//		return ;
//	}
//}