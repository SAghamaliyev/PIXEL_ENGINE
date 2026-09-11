#include "TextureManager.h"
#include "../../Logger/Logger.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void TextureManager::readBinary(uint64_t& TextureID, unsigned char*& data, int& width, int& height, int& nrChannels) {
	const fs::path binPath = fs::path("src") / "bins" / (std::to_string(TextureID) + ".bin");

	if (!fs::exists(binPath)) {
		Logger::addLog(LOG_ERROR, "Binary texture file does not exist: " + binPath.string());
		data = nullptr;
		width = 0;
		height = 0;
		nrChannels = 0;
		return;
	}

	std::ifstream file(binPath, std::ios::binary);
	if (!file) {
		Logger::addLog(LOG_ERROR, "Cannot open binary file: " + binPath.string());
		data = nullptr;
		width = 0;
		height = 0;
		nrChannels = 0;
		return;
	}

	// Read header (width, height, channels)
	file.read(reinterpret_cast<char*>(&width), sizeof(width));
	file.read(reinterpret_cast<char*>(&height), sizeof(height));
	file.read(reinterpret_cast<char*>(&nrChannels), sizeof(nrChannels));

	if (!file.good()) {
		Logger::addLog(LOG_ERROR, "Failed to read texture header: " + binPath.string());
		data = nullptr;
		return;
	}

	if (width <= 0 || height <= 0 || nrChannels <= 0) {
		Logger::addLog(LOG_ERROR, 
			"Invalid texture dimensions: width=" + std::to_string(width) + 
			", height=" + std::to_string(height) + 
			", channels=" + std::to_string(nrChannels));
		data = nullptr;
		return;
	}

	// Calculate data size and allocate memory
	size_t dataSize = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(nrChannels);
	data = new unsigned char[dataSize];

	// Read texture data
	file.read(reinterpret_cast<char*>(data), dataSize);

	if (!file) {
		Logger::addLog(LOG_ERROR,
			"Failed to read texture data: " + binPath.string());
		delete[] data;
		data = nullptr;
		return;
	}
}

void TextureManager::makeTexture(uint64_t& TextureID) {
	if (TextureID > 0) {
		int width, height, nrChannels;
		unsigned char* data = nullptr;

		Logger::addLog(LOG_INFO, 
			"Loading texture with ID: " + std::to_string(TextureID));

		readBinary(TextureID, data, width, height, nrChannels);
		if (data == nullptr) {
			Logger::addLog(LOG_ERROR,
				"Texture data is empty. ID: " + std::to_string(TextureID));
			return;
		}

		GLenum internalFormat = GL_RGB;
		GLenum dataFormat = GL_RGB;

		if (nrChannels == 1)
		{
			internalFormat = GL_RED;
			dataFormat = GL_RED;
		}
		else if (nrChannels == 3)
		{
			internalFormat = GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrChannels == 4)
		{
			internalFormat = GL_RGBA;
			dataFormat = GL_RGBA;
		}

		unsigned int texture = 0;
		if (data) {
			Logger::addLog(LOG_INFO, 
				"Texture loaded successfully. Dimensions: " + std::to_string(width) + 
				"x" + std::to_string(height) + ", Channels: " + std::to_string(nrChannels));

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			TextureList[TextureID] = texture;
		}
		else {
			Logger::addLog(LOG_ERROR, "Something went wrong with creating Texture");
		}
		delete[] data;
	}
	else {
		Logger::addLog(LOG_ERROR, "Texture ID cannot be negative or equal to 0");
		return;
	}
}

unsigned int TextureManager::getTexture(uint64_t& TextureID, bool& isActive) {
	if (isActive) {
		auto it = TextureList.find(TextureID);

		if (it == TextureList.end()) {
	
			makeTexture(TextureID);
			auto result = TextureList.find(TextureID);
			if (result != TextureList.end()) {

				return result->second;
			}
			Logger::addLog(LOG_ERROR,
				"Failed to load texture. ID: " + std::to_string(TextureID));

			return 0;
		}
		else {
			return it->second;
		}
	}
	// Деактивация - очистка текстуры
	else {
		auto it = TextureList.find(TextureID);
		if (it != TextureList.end()) {
			glDeleteTextures(1, &(it->second));
			Logger::addLog(LOG_INFO,
				"Texture deleted. ID: " + std::to_string(TextureID));
			TextureList.erase(it);
		}
		return 0;
	}
}

TextureManager::~TextureManager() {
	for (auto it = TextureList.begin(); it != TextureList.end(); ++it) {
		glDeleteTextures(1, &(it->second));
	}
	TextureList.clear();
}
