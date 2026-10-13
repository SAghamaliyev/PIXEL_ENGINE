#include "ShaderManager.h"

#include <filesystem>
#include <fstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Logger/Logger.h"

using namespace std;
namespace fs = std::filesystem;

void ShaderManager::createShader(const string& vertexSource, const string& fragmentSource) {
    const char* vertex_shader_source = vertexSource.c_str();
    const char* fragment_shader_source = fragmentSource.c_str();

    // Process of vertex shader's creating 
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    int vertex_compile_status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_compile_status);

    if (vertex_compile_status != GL_TRUE) {  // Delete the shader and stop if compilation fails
        char log_info[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, log_info);
        Logger::addLog(LogType::LOG_ERROR, log_info);
        glDeleteShader(vertex_shader);
        return;
    }
    Logger::addLog(LogType::LOG_DEBUG, "Vertex Shader is compiled successfully");

    // Process of fragment shader's creating 
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    int fragment_compile_status;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_compile_status);

    if (fragment_compile_status != GL_TRUE) {  // Delete the shader and stop if compilation fails
        char log_info[512];
        glGetShaderInfoLog(fragment_shader, 512, NULL, log_info);
        Logger::addLog(LogType::LOG_ERROR, log_info);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return;
    }
    Logger::addLog(LogType::LOG_DEBUG, "Fragment Shader is compiled successfully");

    // Process of vertex and fragment shaders' linking
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    int program_link_status;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &program_link_status);

    if (program_link_status != GL_TRUE) {  // Delete the shader and stop if linking fails
        char log_info[512];
        glGetProgramInfoLog(shader_program, 512, NULL, log_info);
        Logger::addLog(LogType::LOG_ERROR, log_info);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteProgram(shader_program);
        return;
    }
    Logger::addLog(LogType::LOG_DEBUG, "Program Shader is linked successfully");

    // Since we need only linked program, we can delete both of these shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    m_shaderMap[MaterialType::Default] = shader_program;
}

ShaderManager::ShaderManager() {

    string vertex_source;
    string fragment_source;

    const fs::path shaders_dir = fs::path("assets") / "Shaders";
    if (!fs::exists(shaders_dir)) {
        Logger::addLog(LogType::LOG_ERROR, "Shaders directory not found: " + shaders_dir.string());
        return;
    }

    try {
        // Reading file
        for (const auto& entry : fs::directory_iterator(shaders_dir)) {
            if (!entry.is_regular_file()) {
                continue;
            }

            fs::path vertex_path = entry.path();

            // We are only looking for .vert files
            if (vertex_path.extension().string() != ".vert") {
                continue;
            }

            // A vertex shader is valid only when its matching("twin") fragment shader exists
            fs::path fragment_path = vertex_path;
            fragment_path.replace_extension(".frag");
            if (!fs::exists(fragment_path)) {
                continue;
            }

            Logger::addLog(LogType::LOG_INFO, "Loading vertex shader: " + vertex_path.string());
            Logger::addLog(LogType::LOG_INFO, "Loading fragment shader: " + fragment_path.string());


            ifstream vert_file(vertex_path);
            if (!vert_file.is_open()) {
                Logger::addLog(LogType::LOG_ERROR,
                    "Can't open shader from Shaders folder" + vertex_path.filename().string());

                return;
            }

            ifstream frag_file(fragment_path);
            if (!frag_file.is_open()) {
                Logger::addLog(LogType::LOG_ERROR,
                    "Can't open shader from Shaders folder" + fragment_path.filename().string());

                return;
            }

            // If both of them exist we start creating our shader program 
            string line;
            string final_line;
            while (getline(vert_file, line)) {
                line += "\n";
                final_line += line;
            }
            vertex_source = final_line;

            line = final_line = "";
            while (getline(frag_file, line)) {
                line += "\n";
                final_line += line;
            }
            fragment_source = final_line;

            createShader(vertex_source, fragment_source);
        }
    }
    catch(...){
        Logger::addLog(LogType::LOG_ERROR, "Something went wrong");
    }
}

unsigned int ShaderManager::getShader(MaterialType materialType) {

    auto it = m_shaderMap.find(materialType);
    if (it != m_shaderMap.end()) {
        return it->second;
    }
    else {
        return m_shaderMap[MaterialType::Default];
    }
}

ShaderManager::~ShaderManager() {
    for (auto it = m_shaderMap.begin(); it != m_shaderMap.end(); it++) {
        glDeleteProgram(it->second);
    }
}
