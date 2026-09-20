#pragma once
#include "../../Core/Logger/Logger.h"

#include <fstream>
#include <filesystem>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <unordered_map>

class TextureManager {
public:
	unsigned int getTexture(uint64_t TextureID, bool isActive);
	~TextureManager();
private:
	std::unordered_map<uint64_t, unsigned int> TextureList;
	void makeTexture(uint64_t TextureID);
	void readBinary(uint64_t TextureID, unsigned char*& data, int& width, int& height, int& nrChannels);
};
