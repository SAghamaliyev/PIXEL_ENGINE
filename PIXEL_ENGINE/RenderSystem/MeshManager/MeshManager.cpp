#include "MeshManager.h"
#include "MeshManager.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>

void MeshManager::readBinaryMesh(unsigned long long int MeshID, vector<float>& vertices,
    vector<int>& indices, vector<float>& textures) {
    const filesystem::path binaryPath = filesystem::path("src") / "bins" /
        (to_string(MeshID) + ".bin");

    ifstream binaryFile(binaryPath, ios::binary);
    if (!binaryFile.is_open()) {
        throw runtime_error("Failed to open binary mesh: " + binaryPath.string());
    }

    uint32_t verticesCount = 0;
    uint32_t indicesCount = 0;
    uint32_t texturesCount = 0;

    binaryFile.read(reinterpret_cast<char*>(&verticesCount), sizeof(verticesCount));
    binaryFile.read(reinterpret_cast<char*>(&indicesCount), sizeof(indicesCount));
    binaryFile.read(reinterpret_cast<char*>(&texturesCount), sizeof(texturesCount));
    if (!binaryFile.good()) {
        throw runtime_error("Invalid binary mesh header: " + binaryPath.string());
    }

    vertices.resize(verticesCount);
    indices.resize(indicesCount);
    textures.resize(texturesCount);

    binaryFile.read(reinterpret_cast<char*>(vertices.data()),
        static_cast<streamsize>(vertices.size() * sizeof(float)));
    binaryFile.read(reinterpret_cast<char*>(indices.data()),
        static_cast<streamsize>(indices.size() * sizeof(int)));
    binaryFile.read(reinterpret_cast<char*>(textures.data()),
        static_cast<streamsize>(textures.size() * sizeof(float)));

    if (!binaryFile.good()) {
        throw runtime_error("Invalid binary mesh data: " + binaryPath.string());
    }
}



MeshInfo MeshManager::makeMesh(unsigned long long int& MeshID) {
    vector <float> vertices;
    vector <int> indices;
    vector <float> textures;

    readBinaryMesh(MeshID, vertices, indices, textures);

    unsigned int VAO, VBO_Pos,VBO_Tex, EBO;
    glGenVertexArrays(1, &VAO);//набор инструкций для работы с буффером с данными

    // Генерируем буфферы
    glGenBuffers(1, &VBO_Pos);
    glGenBuffers(1, &VBO_Tex);

    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO_Pos);//инициализируем буффер и засовываем данные
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // position attribute(how to read VBO_Pos)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Tex);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), textures.data(), GL_STATIC_DRAW);

    // position attribute(how to read VBO_Tex)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
   


    glBindVertexArray(0); // говорим что конец инструкции

    MeshInfo result;
    result = { VAO,VBO_Pos,EBO,(unsigned int)indices.size()};

    return result;
}

MeshInfo MeshManager::getMesh(unsigned long long int& MeshID, bool& isActive) {

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
        MeshInfo result = {};
        result.indexcount = -2007;

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
        glDeleteBuffers(1, &(it->second.VBO));
        glDeleteBuffers(1, &(it->second.EBO));
    }
}