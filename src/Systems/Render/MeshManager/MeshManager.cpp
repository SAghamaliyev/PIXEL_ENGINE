#include "MeshManager.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Definitions/CommonDefinitions.h"
#include "Core/Logger/Logger.h"

using namespace std;
namespace fs = std::filesystem;

void MeshManager::readBinaryMesh(uint64_t meshID,
                                 vector<float>& vertices,
                                 vector<unsigned int>& indices, 
                                 vector<float>& textures) {

    // We read only .obj files' bins
    const fs::path binary_path = fs::path("assets") / "bins" / (to_string(meshID) + ".bin");

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

    // Generate Buffers
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

    // Setting instructions to VAO
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

    // Indices
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

void MeshManager::makeDefaultMesh() {
    std::vector<float> vertices = {
        // Задняя грань
        -0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,   0.5f,  0.5f, -0.5f,  -0.5f,  0.5f, -0.5f,
        // Передняя грань
        -0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,
        // Левая грань
        -0.5f,  0.5f,  0.5f,  -0.5f,  0.5f, -0.5f,  -0.5f, -0.5f, -0.5f,  -0.5f, -0.5f,  0.5f,
        // Правая грань
         0.5f,  0.5f,  0.5f,   0.5f,  0.5f, -0.5f,   0.5f, -0.5f, -0.5f,   0.5f, -0.5f,  0.5f,
        // Нижняя грань
        -0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,   0.5f, -0.5f,  0.5f,  -0.5f, -0.5f,  0.5f,
        // Верхняя грань
        -0.5f,  0.5f, -0.5f,   0.5f,  0.5f, -0.5f,   0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f
    };
    std::vector<float> textures = {
        0,0, 1,0, 1,1, 0,1,
        0,0, 1,0, 1,1, 0,1,
        1,0, 1,1, 0,1, 0,0,
        1,0, 1,1, 0,1, 0,0,
        0,1, 1,1, 1,0, 0,0,
        0,1, 1,1, 1,0, 0,0
    };
    std::vector<unsigned int> indices = {
         0, 1, 2,  2, 3, 0,
         4, 5, 6,  6, 7, 4,
         8, 9,10, 10,11, 8,
        12,13,14, 14,15,12,
        16,17,18, 18,19,16,
        20,21,22, 22,23,20
    };


    unsigned int vao, vbo_pos, vbo_tex, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo_pos);
    glGenBuffers(1, &vbo_tex);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex);
    glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(float), textures.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
    MeshInfo cubeInfo;
    cubeInfo.vao = vao;
    cubeInfo.vboPosition = vbo_pos;
    cubeInfo.vboTexture = vbo_tex;
    cubeInfo.ebo = ebo;
    cubeInfo.indexCount = indices.size();

    m_meshMap[MESH_DEFAULT_ID] = cubeInfo;
}

MeshInfo MeshManager::getMesh(uint64_t meshID, bool isActive) {

    // If Entity is active
    if (isActive) {
        // If the entity's mesh isn't loaded yet, load it
        if (m_meshMap.find(meshID) == m_meshMap.end()) {
            if (meshID == MESH_DEFAULT_ID) {
                makeDefaultMesh();
                return m_meshMap[meshID];
            }

            makeMesh(meshID);

            auto result = m_meshMap.find(meshID);
            if (result != m_meshMap.end()) {
                return result->second;
            }

            // Loading failed: return an empty mesh (vao == 0)
            else {
                Logger::addLog(LogType::LOG_ERROR, "Failed to load mesh. ID: " + std::to_string(meshID));
                MeshInfo empty;
                empty.vao = 0;
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
        empty.vao = 0;
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