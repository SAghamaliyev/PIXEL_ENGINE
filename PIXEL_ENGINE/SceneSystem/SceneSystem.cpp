#include "SceneSystem.h"

#include <algorithm>
#include <cctype>
#include <filesystem>

namespace {

bool isObjectAsset(const std::filesystem::path& path) {
	string extension = path.extension().string();
	transform(extension.begin(), extension.end(), extension.begin(),
		[](unsigned char character) { return static_cast<char>(tolower(character)); });
	return extension == ".obj" || extension == ".fbx";
}

}

SceneSystem::SceneSystem()
	:OurSceneInfo{OurEntityManager.getEntityList()}
{
}

void SceneSystem::SceneClearEntityList() {
	OurEntityManager.clearEntityList();
}

void SceneSystem::SceneCreateEntity(const string& Path, MaterialType Type, const string& Name) {
	OurEntityManager.createEntity(Path, Type, Name);
}

//void SceneSystem::SceneCreateEntity(MaterialType Type, const string& Name) {
//	OurEntityManager.createEntity(Type, Name);
//}

void SceneSystem::SceneDeleteEntity(unsigned int EntityID) {
	OurEntityManager.deactivateEntity(EntityID);
}

EntityUnit& SceneSystem::SceneGetEntity(unsigned int EntityID) {
	return OurEntityManager.getEntity(EntityID);
}

const SceneInfo& SceneSystem::getSceneInfo() const {
	return OurSceneInfo;
}

bool SceneSystem::ChangeColorEntity(unsigned int EntityID, Color targetColor) {
	return OurEntityManager.changeColor(EntityID, targetColor);
}

bool SceneSystem::SceneHasEntity(unsigned int EntityID) const {
	return OurEntityManager.hasEntity(EntityID);
}

bool SceneSystem::SceneRenameEntity(unsigned int EntityID, const string& Name) {
   if (!OurEntityManager.hasEntity(EntityID) || Name.empty()) {
		return false;
	}

	EntityUnit& entity = OurEntityManager.getEntity(EntityID);
	const std::filesystem::path oldPath(entity.path);

	if (isObjectAsset(oldPath)) {
        if (!OurEntityManager.renameEntityFile(EntityID, Name)) {
			return false;
		}
	}

	return OurEntityManager.renameEntity(EntityID, Name);
}

bool SceneSystem::SceneSetEntityMaterial(unsigned int EntityID, MaterialType Type) {
	return OurEntityManager.setEntityMaterial(EntityID, Type);
}

void SceneSystem::SceneUpdate() {
	OurEntityManager.Update();
}
