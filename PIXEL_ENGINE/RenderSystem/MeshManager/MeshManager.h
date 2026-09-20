#pragma once
#include "../../Core/Definitions/SceneDefinitons.h"
#include "../../Core/Logger/Logger.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <unordered_map>

class MeshManager {
public:
	MeshInfo getMesh(uint64_t MeshID, bool isActive);
    ~MeshManager();
private:
	std::unordered_map <uint64_t /*MeshID*/, MeshInfo /*OurMesh*/> MeshMap;
	MeshInfo makeMesh(uint64_t MeshID);
	
	// Read all info from Objects' .bin files 
	void readBinaryMesh(uint64_t MeshID, std::vector<float>& vertices,
		std::vector<unsigned int>& indices, std::vector<float>& textures);
};
