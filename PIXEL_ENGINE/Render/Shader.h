#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

using namespace std;

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

class ShaderManager {
private:
    unordered_map <MaterialType, unsigned int> ShaderMap;
public:
    ShaderManager();
    unsigned int getShader(MaterialType Material);
};