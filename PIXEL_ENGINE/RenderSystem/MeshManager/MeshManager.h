#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "../../src/Definitions.h"

using namespace std;


class MeshManager {
public:
    MeshManager() = default;
	MeshInfo getMesh(unsigned long long int& MeshID, bool& isActive);

    ~MeshManager();
private:
	unordered_map <unsigned long long int /*MeshID*/, MeshInfo /*OurMesh*/> MeshMap;
	MeshInfo makeMesh(unsigned long long int& MeshID);

	void readBinaryMesh(unsigned long long int MeshID, vector<float>& vertices,
		vector<int>& indices, vector<float>& textures);
};