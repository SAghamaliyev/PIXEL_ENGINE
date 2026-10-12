#include "TextureManager.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Logger/Logger.h"

using namespace std;
namespace fs = std::filesystem;

void TextureManager::readBinaryTexture(uint64_t textureID,
                                       unsigned char*& data,
                                       int& width,
                                       int& height,
                                       int& nrChannels) {

    const fs::path bin_path = fs::path("assets") / "bins" / (std::to_string(textureID) + ".bin");

    if (!fs::exists(bin_path)) {
        Logger::addLog(LogType::LOG_ERROR, "Binary texture file does not exist: " + bin_path.string());
        data = nullptr;
        width = 0;
        height = 0;
        nrChannels = 0;
        return;
    }

    std::ifstream file(bin_path, std::ios::binary);
    if (!file) {
        Logger::addLog(LogType::LOG_ERROR, "Cannot open binary file: " + bin_path.string());
        data = nullptr;
        width = 0;
        height = 0;
        nrChannels = 0;
        return;
    }

    // Read texture dimensions and channel count
    file.read(reinterpret_cast<char*>(&width), sizeof(width));
    file.read(reinterpret_cast<char*>(&height), sizeof(height));
    file.read(reinterpret_cast<char*>(&nrChannels), sizeof(nrChannels));

    if (!file.good()) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to read texture header: " + bin_path.string());
        data = nullptr;
        return;
    }

    if (width <= 0 || height <= 0 || nrChannels <= 0) {
        Logger::addLog(LogType::LOG_ERROR,
            "Invalid texture dimensions: width=" + std::to_string(width) +
            ", height=" + std::to_string(height) +
            ", channels=" + std::to_string(nrChannels));
        data = nullptr;
        return;
    }

    // Calculate data size and allocate memory
    size_t data_size = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(nrChannels);
    data = new unsigned char[data_size];

    // Read texture data
    file.read(reinterpret_cast<char*>(data), data_size);

    if (!file) {
        Logger::addLog(LogType::LOG_ERROR,
            "Failed to read texture data: " + bin_path.string());
        delete[] data;
        data = nullptr;
        return;
    }
}

void TextureManager::makeTexture(uint64_t textureID) {
    if (textureID == 0) {
        Logger::addLog(LogType::LOG_ERROR, "Texture ID cannot be equal to 0");
        return;
    }

    int width, height, nrChannels;
    unsigned char* data = nullptr;

    Logger::addLog(LogType::LOG_INFO, "Loading texture with ID: " + std::to_string(textureID));

    readBinaryTexture(textureID, data, width, height, nrChannels);
    if (data == nullptr) {
        Logger::addLog(LogType::LOG_ERROR, "Texture data is empty. ID: " + std::to_string(textureID));
        return;
    }

    GLenum format = GL_RGB;

    if (nrChannels == 1) {
        format = GL_RED;
    }
    else if (nrChannels == 3) {
        format = GL_RGB;
    }
    else if (nrChannels == 4) {
        format = GL_RGBA;
    }
    else {
        Logger::addLog(LogType::LOG_ERROR, "Unsupported number of channels. ID: " + std::to_string(textureID));
        delete[] data;
        return;
    }

    Logger::addLog(LogType::LOG_INFO, "Texture loaded successfully!");

    unsigned int texture = 0;

    // Settings for future texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 format,
                 width,
                 height,
                 0,
                 format,
                 GL_UNSIGNED_BYTE,
                 data);

    glGenerateMipmap(GL_TEXTURE_2D);

    m_textureMap[textureID] = texture;

    delete[] data;
}

unsigned int TextureManager::getTexture(uint64_t textureID, bool isActive) {

    // If Entity is active 
    if (isActive) {
        // If no any texture was found for Entity, we must create it
        if (m_textureMap.find(textureID) == m_textureMap.end()) {
            makeTexture(textureID);

            auto result = m_textureMap.find(textureID);
            if (result != m_textureMap.end()) {

                return result->second;
            }
            // If creation of texture failed
            else {
                Logger::addLog(LogType::LOG_ERROR, "Failed to load texture. ID: " + std::to_string(textureID));
                return 0;
            }
        }

        return m_textureMap[textureID];
    }
    // If Entity is diactivated
    else {
        auto it = m_textureMap.find(textureID);

        // If Entity's texture is still in map, we must delete it
        if (it != m_textureMap.end()) {
            glDeleteTextures(1, &(it->second));
            Logger::addLog(LogType::LOG_INFO, "Texture deleted. ID: " + std::to_string(textureID));
            m_textureMap.erase(it);
        }
        return 0;
    }
}

TextureManager::~TextureManager() {
    for (auto it = m_textureMap.begin(); it != m_textureMap.end(); ++it) {
        glDeleteTextures(1, &(it->second));
    }
    m_textureMap.clear();
}
