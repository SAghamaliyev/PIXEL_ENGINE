#include "EntityManager.h"

#include <filesystem>
using namespace std;

EntityManager::EntityManager() {

}

void EntityManager::createEntity(uint64_t MeshID, MaterialType ShaderID,
								const string& Name) {
	EntityUnit OurEntity;
	OurEntity.EntityID = counter;
	OurEntity.nCopies = 0;
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

void EntityManager::duplicateEntity(long EntityID) {
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

bool EntityManager::hasEntity(long EntityID) {
	auto it = EntityList.find(EntityID);

	if (it == EntityList.end() && it->second.isActive)
		return false;
	return true;
}

void EntityManager::deactivateEntity(long EntityID) {
	if (hasEntity(EntityID)) {
		EntityList[EntityID].isActive = false;
		DeactivatedList.push_back(EntityID);
	}
	else {
		Logger::addLog(LOG_ERROR, "Entity is either already deactivated or not exist");
		return;
	}
}

void EntityManager::makeActive(long EntityID) {
	
	auto& it = EntityList.at(EntityID);
	
	if (it.isActive) {
		OurActiveEntityID = it.EntityID;
	}
}

void EntityManager::Update() {

	// Clear all deactivated entities
	for (auto element : DeactivatedList) {
		EntityList.erase(element);
	}

	DeactivatedList.clear();
}

void EntityManager::changeColor(long EntityID, const Color& targetColor) {
	if (hasEntity(EntityID)) {
		EntityList[EntityID].color = targetColor;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Color: " + EntityList[EntityID].name);
		return;
	}
}

void EntityManager::changeName(long EntityID, const string& Name) {
	if (hasEntity(EntityID)) {
		EntityList[EntityID].name = Name;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Name: " + EntityList[EntityID].name);
		return;
	}
}

void EntityManager::changeMaterial(long EntityID, MaterialType ShaderID) {
	if (hasEntity(EntityID)) {
		EntityList[EntityID].ShaderID = ShaderID;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Material: " + EntityList[EntityID].name);
		return;
	}
}

void EntityManager::changeMesh(long EntityID, uint64_t MeshID) {
	if (hasEntity(EntityID)) {
		EntityList[EntityID].MeshID = MeshID;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Mesh: " + EntityList[EntityID].name);
		return;
	}
}

void EntityManager::changeTexture(long EntityID, uint64_t TextureID) {
	if (hasEntity(EntityID)) {
		EntityList[EntityID].TextureID = TextureID;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Texture: " + std::to_string(EntityID));
		return;
	}
}

void EntityManager::changeTranslate(long EntityID, const glm::vec3& TranslateV) {
	if (hasEntity(EntityID)) {
		EntityList[EntityID].TransformInfo.TranslateV = TranslateV;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Transform Translate: " + std::to_string(EntityID));
		return;
	}
}

void EntityManager::changeRotate(long EntityID, const glm::vec3& RotateV) {
	if (hasEntity(EntityID)) {
		EntityList[EntityID].TransformInfo.RotateV = RotateV;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Transform Rotate: " + std::to_string(EntityID));
		return;
	}
}

void EntityManager::changeScale(long EntityID, const glm::vec3& ScaleV) {
	if (hasEntity(EntityID)) {
		EntityList[EntityID].TransformInfo.ScaleV = ScaleV;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and change its Transform Scale: " + std::to_string(EntityID));
		return;
	}
}

void EntityManager::activateColor(long EntityID) {
	if (hasEntity(EntityID)) {
		EntityList[EntityID].isColorActive = true;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and activate its Color: " + std::to_string(EntityID));
		return;
	}
}

void EntityManager::deactivateColor(long EntityID) {
	if (hasEntity(EntityID)) {
		EntityList[EntityID].isColorActive = false;
	}
	else {
		Logger::addLog(LOG_ERROR,
			"Failed to find entity and deactivate its Color: " + std::to_string(EntityID));
		return;
	}
}

long& EntityManager::getActiveEntityID() {
	return OurActiveEntityID;
}

EntityUnit& EntityManager::getEntity(long EntityID) {
	return EntityList.at(EntityID);
}

const unordered_map <long, EntityUnit>& EntityManager::getEntityList() const {
	return EntityList;
}

const Transform& EntityManager::getTransformInfo(long EntityID) const {
	return EntityList.at(EntityID).TransformInfo;
}