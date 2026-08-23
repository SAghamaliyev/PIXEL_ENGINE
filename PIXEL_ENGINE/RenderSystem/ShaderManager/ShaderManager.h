#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <unordered_map>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "../../src/Definitions.h"

using namespace std;

class ShaderManager {
private:
    unordered_map <MaterialType, unsigned int> ShaderMap;
public:
    ShaderManager();
    unsigned int getShader(MaterialType Material);
};