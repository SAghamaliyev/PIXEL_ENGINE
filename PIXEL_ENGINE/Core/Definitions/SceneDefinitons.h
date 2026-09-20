#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include <../../glm-1.0.3/glm/glm.hpp>
#include <../../glm-1.0.3/glm/gtc/matrix_transform.hpp>
#include <../../glm-1.0.3/glm/gtc/type_ptr.hpp>

enum MaterialType {
    Default = 0,     // Серый/белый дефолтный материал для отладки
    Unlit,           // Без освещения (для UI, превью, плоских цветов)
    PBR_Standard,    // Стандартный PBR (Albedo, Roughness, Metalness, Normal)
    BlinnPhong,      // Классический Blinn-Phong (Diffuse, Specular, Shininess)
    Glass,           // Прозрачный материал (требует блендинга и сортировки)
    Water,           // Вода (с анимацией вершин/текстур через шейдер)
    Terrain,         // Ландшафт (с поддержкой блендинга текстур по маске/слоям)
    Skybox,          // Специальный шейдер для скайбокса
    Custom           // Заглушка под динамические/кастомные шейдеры
};

struct Color {
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
};

struct MeshInfo {
    bool isActive = true;
    unsigned int VAO = 0;
    unsigned int VBO_Pos = 0;
    unsigned int VBO_Tex = 0;
    unsigned int EBO = 0;
    unsigned int indexCount = 0;
    unsigned int textureCount = 0;
};


struct Transform {
    glm::mat4 OurMatrix = glm::mat4(1.0f);
    glm::vec3 RotateV = glm::vec3(0.0f);
    glm::vec3 TranslateV = glm::vec3(0.0f);
    glm::vec3 ScaleV = glm::vec3(1.0f);
};


struct EntityUnit {
    unsigned int EntityID;
    unsigned int nCopies;	// Numbers of copies of one entity
    uint64_t MeshID;
    uint64_t TextureID;
    MaterialType ShaderID;	// Each Shader is working with Entity's material(for liquid we have liquid shader, for solid solid and etc.)

    Color color;
    std::string name;
    Transform TransformInfo;

    bool isActive;	// Should we render it or not
    bool isColorActive;	// Should we add color to it or not(defaultly no)
};

struct CameraUnit {
    unsigned int CameraID;

    // We add this matrix to make object look from our perspective(view)
    glm::mat4 View;

    // We add this matrix to stop rendering other parts of screen which are out of our view
    glm::mat4 Projection;

    glm::mat4 OurMatrix;

    glm::vec3 TransV;  // Speed of movement Camera

    float FOVdegree;
    float near;
    float far;
};

struct SceneInfo {
    const std::unordered_map <unsigned int, EntityUnit>& EntityList;
    const CameraUnit& OurActiveCamera;
};