#pragma once

#include <string>
#include <unordered_map>

#include "Core/Definitions/MaterialDefinitions.h"

class ShaderManager {
private:
    std::unordered_map <MaterialType, unsigned int> m_shaderMap;
    void createShader(const std::string& vertexSource, const std::string& fragmentSource);
public:
    // Create all active shaders during initialization so they are available before rendering begins
    ShaderManager(); 

    ~ShaderManager();
    unsigned int getShader(MaterialType materialType);
};