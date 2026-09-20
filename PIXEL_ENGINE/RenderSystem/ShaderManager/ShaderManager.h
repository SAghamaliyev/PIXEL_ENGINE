#pragma once
#include "../../Core/Definitions/SceneDefinitons.h"
#include "../../Core/Logger/Logger.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <unordered_map>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

class ShaderManager {
private:
    std::unordered_map <MaterialType, unsigned int> ShaderMap;
    void createShader(std::string& vertexSourceStr, std::string& fragmentSourceStr);
public:
    ShaderManager();    // When we create shadermanager it starts creating all of active shaders beforehead
    ~ShaderManager();
    unsigned int getShader(MaterialType Material);
};