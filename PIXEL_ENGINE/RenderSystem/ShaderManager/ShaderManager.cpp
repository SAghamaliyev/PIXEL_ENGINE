#include "ShaderManager.h"
#include "../../Logger/Logger.h"

namespace fs = std::filesystem;

void ShaderManager::createShader(string& vertexSourceStr, string& fragmentSourceStr) {
	const char* vertexShaderSource = vertexSourceStr.c_str();
	const char* fragmentShaderSource = fragmentSourceStr.c_str();

	Logger::addLog(LOG_DEBUG, "Vertex Shader was added!!");
	Logger::addLog(LOG_DEBUG, "Fragment Shader was added!!");

	// Making Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int vertex_compiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertex_compiled);
	if (vertex_compiled != GL_TRUE) {
		char logInfo[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, logInfo);
		Logger::addLog(LOG_ERROR, logInfo);
		glDeleteShader(vertexShader);
		return;
	}
	else { Logger::addLog(LOG_DEBUG, "Vertex Shader is compiled successfully"); }

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int fragment_compiled;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragment_compiled);
	if (fragment_compiled != GL_TRUE) {
		char logInfo[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, logInfo);
		Logger::addLog(LOG_ERROR, logInfo);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return;
	}
	else { Logger::addLog(LOG_DEBUG, "Fragment Shader is compiled successfully"); }

	unsigned int programShader = glCreateProgram();
	glAttachShader(programShader, vertexShader);
	glAttachShader(programShader, fragmentShader);
	glLinkProgram(programShader);

	int program_compiled;
	glGetProgramiv(programShader, GL_LINK_STATUS, &program_compiled);

	if (program_compiled != GL_TRUE) {
		char logInfo[512];
		glGetProgramInfoLog(programShader, 512, NULL, logInfo);
		Logger::addLog(LOG_ERROR, logInfo);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(programShader);
		return;
	}
	else { Logger::addLog(LOG_DEBUG, "Program Shader is linked successfully"); }


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	ShaderMap[Default] = programShader;
}

ShaderManager::ShaderManager() {

    string vertexSourceStr;
    string fragmentSourceStr;

    // Reading file
    for (const auto& entry : fs::directory_iterator("src/Shaders")) {
        if (!entry.is_regular_file()) {
            continue;
        }

        fs::path vertPath = entry.path();
	
        // We are looking for only for .vert files(a.k.a our entry points)
        if (vertPath.extension().string() != ".vert") {
            continue;	// If it's .frag shader we skip it
        }

        // We must be sure that there is also its fragment("twin") part(shader)
        fs::path fragPath = vertPath;
        fragPath.replace_extension(".frag");
        if (!fs::exists(fragPath)) {
            continue;
        }

        Logger::addLog(
            LOG_INFO,
            "Loading vertex shader: " + vertPath.string());

        Logger::addLog(
            LOG_INFO,
            "Loading fragment shader: " + fragPath.string());


        ifstream vertFile(vertPath);
        if (!vertFile.is_open()) {
            Logger::addLog(LOG_ERROR, "Can't open shader from Shaders folder" + vertPath.filename().string());
            return;
        }

        ifstream fragFile(fragPath);
        if (!fragFile.is_open()) {
            Logger::addLog(LOG_ERROR, "Can't open shader from Shaders folder" + fragPath.filename().string());
            return;
        }

        // If both of them exist we start creating our common shader
        string line;
        string finalLine;
        while (getline(vertFile, line)) {
            line += "\n";
            finalLine += line;
        }
        vertexSourceStr = finalLine;

        line = finalLine = "";
        while (getline(fragFile, line)) {
            line += "\n";
            finalLine += line;
        }
        fragmentSourceStr = finalLine;

        createShader(vertexSourceStr, fragmentSourceStr);
		
    }
}

unsigned int ShaderManager::getShader(MaterialType Material) {

	auto it = ShaderMap.find(Material);
	if (it != ShaderMap.end()) {
		return it->second;	// ~ShaderMap[Material]
	}
	else {
		return ShaderMap[Default];
	}
}

ShaderManager::~ShaderManager() {
	for (auto it = ShaderMap.begin(); it != ShaderMap.end(); it++) {
		glDeleteProgram(it->second);
	}
}