#include "MeshManager.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>

void MeshManager::readBinaryMesh(uint64_t MeshID, vector<float>& vertices,
    vector<unsigned int>& indices, vector<float>& textures) {

    // We read only .obj files' bins
    const filesystem::path binaryPath = filesystem::path("src") / "bins" /
        (to_string(MeshID) + ".bin");

    ifstream binaryFile(binaryPath, ios::binary);
    if (!binaryFile.is_open()) {
        Logger::addLog(LOG_ERROR, "Failed to open binary file: " + binaryPath.string());
        return;
    }

    uint32_t verticesCount = 0;
    uint32_t indicesCount = 0;
    uint32_t texturesCount = 0;

    binaryFile.read(reinterpret_cast<char*>(&verticesCount), sizeof(verticesCount));
    binaryFile.read(reinterpret_cast<char*>(&indicesCount), sizeof(indicesCount));
    binaryFile.read(reinterpret_cast<char*>(&texturesCount), sizeof(texturesCount));

    if (!binaryFile.good()) {
        Logger::addLog(LOG_ERROR, "Invalid binary mesh header: " + binaryPath.string());
        return;
    }

    vertices.resize(verticesCount);
    indices.resize(indicesCount);
    textures.resize(texturesCount);

    binaryFile.read(reinterpret_cast<char*>(vertices.data()),
        static_cast<streamsize>(vertices.size() * sizeof(float)));
    binaryFile.read(reinterpret_cast<char*>(indices.data()),
        static_cast<streamsize>(indices.size() * sizeof(unsigned int)));
    binaryFile.read(reinterpret_cast<char*>(textures.data()),
        static_cast<streamsize>(textures.size() * sizeof(float)));

    if (!binaryFile.good()) {
        Logger::addLog(LOG_ERROR, "Invalid binary mesh data: " + binaryPath.string());
        return;
    }
}



MeshInfo MeshManager::makeMesh(uint64_t MeshID) {

    vector<float> vertices;
    vector<unsigned int> indices;
    vector<float> textures;

    readBinaryMesh(MeshID, vertices, indices, textures);

    unsigned int VAO, VBO_Pos, VBO_Tex, EBO;

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO_Pos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Pos);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &VBO_Tex);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Tex);
    glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(float), textures.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // Indices Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    MeshInfo result;

    result = { true, VAO, VBO_Pos, VBO_Tex, EBO, (unsigned int)indices.size(), (unsigned int) textures.size()};

    return result;
}

MeshInfo MeshManager::getMesh(uint64_t MeshID, bool isActive) {

    // If Entity is active
    if (isActive) {

        // Nothing was found
        if (MeshMap.find(MeshID) == MeshMap.end()) {
            MeshInfo result = makeMesh(MeshID);
            MeshMap[MeshID] = result;
            return result;
        }
        // Entity was found
        else {
            return MeshMap[MeshID];
        }
    }

    // If Entity is deactivated 
    else {
        MeshInfo result;
        result.isActive = false;

        // If Entity is already not in map
        if (MeshMap.find(MeshID) == MeshMap.end()) {
            return result;
        }
        // If Entity is still in map
        else {
            MeshMap.erase(MeshID);
            return result;
        }
    }
}


MeshManager::~MeshManager() {
    for (auto it = MeshMap.begin(); it != MeshMap.end(); ++it) {
        glDeleteVertexArrays(1, &(it->second.VAO));
        glDeleteBuffers(1, &(it->second.VBO_Pos));
        glDeleteBuffers(1, &(it->second.VBO_Tex));
        glDeleteBuffers(1, &(it->second.EBO));
    }
}