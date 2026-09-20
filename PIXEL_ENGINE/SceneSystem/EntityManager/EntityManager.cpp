#include "EntityManager.h"

#include <filesystem>
using namespace std;

EntityManager::EntityManager() {
	counter = 0;
}

void EntityManager::createEntity(uint64_t MeshID, MaterialType ShaderID,
								const string& Name) {
	EntityUnit OurEntity;
	OurEntity.EntityID = counter;
	OurEntity.MeshID = MeshID;
	OurEntity.ShaderID = ShaderID;
	OurEntity.isActive = true;
	OurEntity.TextureID = TextureDefault;
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

	// Clear all deactivated entities
	for (auto element : DeactivatedList) {
		EntityList.erase(element);
	}

	DeactivatedList.clear();
}

void EntityManager::changeColor(unsigned int EntityID, const Color& targetColor) {
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

void EntityManager::changeMaterial(unsigned int EntityID, MaterialType ShaderID) {
	if (hasEntity(EntityID)) {
		EntityList[EntityID].ShaderID = ShaderID;
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

void EntityManager::changeTranslate(unsigned int EntityID, const glm::vec3& TranslateV) {
	if (hasEntity(EntityID)) {
		EntityList[EntityID].TransformInfo.TranslateV = TranslateV;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Transform Translate: " + std::to_string(EntityID));
		return;
	}
}

void EntityManager::changeRotate(unsigned int EntityID, const glm::vec3& RotateV) {
	if (hasEntity(EntityID)) {
		EntityList[EntityID].TransformInfo.RotateV = RotateV;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Transform Rotate: " + std::to_string(EntityID));
		return;
	}
}

void EntityManager::changeScale(unsigned int EntityID, const glm::vec3& ScaleV) {
	if (hasEntity(EntityID)) {
		EntityList[EntityID].TransformInfo.ScaleV = ScaleV;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Transform Scale: " + std::to_string(EntityID));
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

EntityUnit& EntityManager::getEntity(unsigned int EntityID) {
	return EntityList.at(EntityID);
}

const unordered_map <unsigned int, EntityUnit>& EntityManager::getEntityList() const {
	return EntityList;
}

const Transform& EntityManager::getTransformInfo(unsigned int EntityID) const {
	return EntityList.at(EntityID).TransformInfo;
}