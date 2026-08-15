#include "Meshing.h"
#include "ObjectReader/ReaderOBJ.h"



MeshInfo MeshManager::makeMesh(const string& path) {
    vector <float> vertices;
    vector <int> indices;

    ReaderOBJ::ObjectReader(path, vertices, indices);

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);//набор инструкций для работы с буффером с данными

    // Генерируем буфферы
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);//инициализируем буффер и засовываем данные
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

    // position attribute(how to read VBO)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
   


    glBindVertexArray(0); // говорим что конец инструкции

    MeshInfo result;
    result = { VAO,VBO,EBO,(unsigned int)indices.size()};

    return result;
}

MeshInfo MeshManager::getMesh(const string& path, const string& name) {

    // Nothing was found
    if (MeshMap.find(name) == MeshMap.end()) {
        MeshInfo result = makeMesh(path);
        MeshMap[name] = result;
        return result;
    }
    else {
        return MeshMap[name];
    }
}

MeshManager::~MeshManager() {
    for (auto it = MeshMap.begin(); it != MeshMap.end(); ++it) {
        glDeleteVertexArrays(1, &(it->second.VAO));
        glDeleteBuffers(1, &(it->second.VBO));
        glDeleteBuffers(1, &(it->second.EBO));
    }
}