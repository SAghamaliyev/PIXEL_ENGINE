#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "../../src/Definitions.h"
#include "../../Logger/Logger.h"

using namespace std;


class MeshManager {
public:
	MeshInfo getMesh(uint64_t& MeshID, bool& isActive);
    ~MeshManager();
private:
	unordered_map <uint64_t /*MeshID*/, MeshInfo /*OurMesh*/> MeshMap;
	MeshInfo makeMesh(uint64_t& MeshID);
	
	// Read all info from Objects' .bin files 
	void readBinaryMesh(uint64_t& MeshID, vector<float>& vertices,
		vector<unsigned int>& indices, vector<float>& textures);
};