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
	MeshInfo getMesh(const string& path, unsigned int MeshID, bool isActive);

    ~MeshManager();
private:
	unordered_map <unsigned int /*Name*/, MeshInfo /*OurMesh*/> MeshMap;
	MeshInfo makeMesh(const string& path);
};