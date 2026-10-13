#include "MeshManager.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Logger/Logger.h"

using namespace std;

void MeshManager::readBinaryMesh(uint64_t meshID,
                                 vector<float>& vertices,
                                 vector<unsigned int>& indices, 
                                 vector<float>& textures) {

    // We read only .obj files' bins
    const filesystem::path binary_path = filesystem::path("assets") / "bins" / (to_string(meshID) + ".bin");

    ifstream binary_file(binary_path, ios::binary);
    if (!binary_file.is_open()) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to open binary file: " + binary_path.string());
        return;
    }

    uint32_t vertices_count = 0;
    uint32_t indices_count = 0;
    uint32_t textures_count = 0;

    binary_file.read(reinterpret_cast<char*>(&vertices_count), sizeof(vertices_count));
    binary_file.read(reinterpret_cast<char*>(&indices_count), sizeof(indices_count));
    binary_file.read(reinterpret_cast<char*>(&textures_count), sizeof(textures_count));

    if (!binary_file.good()) {
        Logger::addLog(LogType::LOG_ERROR, "Invalid binary mesh header: " + binary_path.string());
        return;
    }

    vertices.resize(vertices_count);
    indices.resize(indices_count);
    textures.resize(textures_count);

    binary_file.read(reinterpret_cast<char*>(vertices.data()),
        static_cast<streamsize>(vertices.size() * sizeof(float)));
    binary_file.read(reinterpret_cast<char*>(indices.data()),
        static_cast<streamsize>(indices.size() * sizeof(unsigned int)));
    binary_file.read(reinterpret_cast<char*>(textures.data()),
        static_cast<streamsize>(textures.size() * sizeof(float)));

    if (!binary_file.good()) {
        Logger::addLog(LogType::LOG_ERROR, "Invalid binary mesh data: " + binary_path.string());
        return;
    }
}



void MeshManager::makeMesh(uint64_t meshID) {
    if (meshID == 0) {
        Logger::addLog(LogType::LOG_ERROR, "Mesh ID cannot be equal to 0");
        return;
    }

    vector<float> vertices;
    vector<unsigned int> indices;
    vector<float> textures;

    Logger::addLog(LogType::LOG_INFO, "Loading mesh with ID: " + std::to_string(meshID));

    readBinaryMesh(meshID, vertices, indices, textures);

    if (vertices.empty() || indices.empty()) {
        Logger::addLog(LogType::LOG_ERROR, "Mesh data is empty. ID: " + std::to_string(meshID));
        return;
    }

    if (textures.size() / 2 != vertices.size() / 3) {
        Logger::addLog(LogType::LOG_ERROR,
                       "Mesh vertices and texture coordinates mismatch. ID: " + std::to_string(meshID));
        return;
    }

    unsigned int vao = 0;
    unsigned int vbo_position = 0;
    unsigned int vbo_texture = 0;
    unsigned int ebo = 0;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo_position);
    glGenBuffers(1, &vbo_texture);
    glGenBuffers(1, &ebo);

    if (vao == 0 || vbo_position == 0 || vbo_texture == 0 || ebo == 0) {
        Logger::addLog(LogType::LOG_ERROR, "Something went wrong with creating Mesh buffers");

        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo_position);
        glDeleteBuffers(1, &vbo_texture);
        glDeleteBuffers(1, &ebo);
        return;
    }

    glBindVertexArray(vao);

    // Positions
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);

    glBufferData(GL_ARRAY_BUFFER, 
                 vertices.size() * sizeof(float),
                 vertices.data(), 
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, vbo_texture);

    glBufferData(GL_ARRAY_BUFFER, 
                 textures.size() * sizeof(float), 
                 textures.data(), 
                 GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // Indices (must be bound while the VAO is bound, so the VAO remembers it)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int), 
                 indices.data(), 
                 GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    MeshInfo result = { vao, vbo_position, vbo_texture, ebo, indices.size(), textures.size() };

    m_meshMap[meshID] = result;

    Logger::addLog(LogType::LOG_INFO, "Mesh loaded successfully! ID: " + std::to_string(meshID));
}

MeshInfo MeshManager::getMesh(uint64_t meshID, bool isActive) {

    // If Entity is active
    if (isActive) {
        // If no any mesh was found for Entity, we must create it
        if (m_meshMap.find(meshID) == m_meshMap.end()) {
            makeMesh(meshID);

            auto result = m_meshMap.find(meshID);
            if (result != m_meshMap.end()) {
                return result->second;
            }
            // If creation of mesh failed
            else {
                Logger::addLog(LogType::LOG_ERROR, "Failed to load mesh. ID: " + std::to_string(meshID));
                MeshInfo empty;
                empty.indexCount = -1;
                return empty;
            }
        }
        return m_meshMap[meshID];
    }
    // If Entity is deactivated
    else {
        auto it = m_meshMap.find(meshID);

        // If Entity's mesh is still in map, we must delete it
        if (it != m_meshMap.end()) {
            glDeleteVertexArrays(1, &(it->second.vao));
            glDeleteBuffers(1, &(it->second.vboPosition));
            glDeleteBuffers(1, &(it->second.vboTexture));
            glDeleteBuffers(1, &(it->second.ebo));
            Logger::addLog(LogType::LOG_INFO, "Mesh deleted. ID: " + std::to_string(meshID));
            m_meshMap.erase(it);
        }

        MeshInfo empty;
        empty.indexCount = -1;
        return empty;
    }
}


MeshManager::~MeshManager() {
    for (auto it = m_meshMap.begin(); it != m_meshMap.end(); ++it) {
        glDeleteVertexArrays(1, &(it->second.vao));
        glDeleteBuffers(1, &(it->second.vboPosition));
        glDeleteBuffers(1, &(it->second.vboTexture));
        glDeleteBuffers(1, &(it->second.ebo));
    }
}