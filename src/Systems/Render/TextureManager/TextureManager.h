#pragma once

#include <cstdint>
#include <unordered_map>

class TextureManager {
public:
    unsigned int getTexture(uint64_t textureID, bool isActive);
    ~TextureManager();
private:
    std::unordered_map<uint64_t, unsigned int> m_textureMap;
    void makeTexture(uint64_t textureID);

    // It's just a white texture
    void makeDefaultTexture();

    // Read texture data from a .bin file
    void readBinaryTexture(uint64_t textureID,
                           unsigned char*& data,
                           int& width,
                           int& height,
                           int& nrChannels);
};
