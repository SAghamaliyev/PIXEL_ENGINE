#include "ShaderManager.h"
#include "../../Logger/Logger.h"

namespace fs = std::filesystem;

ShaderManager::ShaderManager() {
	string vertexSourceStr;
	string fragmentSourceStr;

	// Reading file
	for (const auto& entry : fs::directory_iterator("src/Shaders")) {
		if (entry.is_regular_file()) {
			fs::path ItsPath = entry.path();

			ifstream file(ItsPath);

			if (!file.is_open()) {
				throw std::runtime_error("Can't open shader in Shader");
				return;
			}
			string line;
			string finalLine;
			while (getline(file, line)) {
				line += "\n";
				finalLine += line;
			}

			if (ItsPath.extension() == ".vert") {
				vertexSourceStr = finalLine;
			}
			else if (ItsPath.extension() == ".frag") {
				fragmentSourceStr = finalLine;
			}
		}
	}

	const char* vertexShaderSource = vertexSourceStr.c_str();
	const char* fragmentShaderSource = fragmentSourceStr.c_str();

	Logger::getInstance().addLog(Logger::LogEntry::LOG_DEBUG, "Vertex Shader was added :)");
	Logger::getInstance().addLog(Logger::LogEntry::LOG_DEBUG, "Fragment Shader was added :)");
	
	// Making Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int vertex_compiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertex_compiled);
	if (vertex_compiled != GL_TRUE) {
		char logInfo[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, logInfo);
		Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR, logInfo);
	}
	else { Logger::getInstance().addLog(Logger::LogEntry::LOG_DEBUG, "Vertex Shader is compiled successfully"); }

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int fragment_compiled;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragment_compiled);
	if (fragment_compiled != GL_TRUE) {
		char logInfo[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, logInfo);
		Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR, logInfo);
	}
	else{ Logger::getInstance().addLog(Logger::LogEntry::LOG_DEBUG, "Fragment Shader is compiled successfully"); }

	unsigned int programShader = glCreateProgram();
	glAttachShader(programShader, vertexShader);
	glAttachShader(programShader, fragmentShader);
	glLinkProgram(programShader);

	int program_compiled;
	glGetProgramiv(programShader, GL_LINK_STATUS, &program_compiled);

	if (program_compiled != GL_TRUE) {
		char logInfo[512];
		glGetProgramInfoLog(programShader, 512, NULL, logInfo);
		Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR, logInfo);
	}
	else { Logger::getInstance().addLog(Logger::LogEntry::LOG_DEBUG, "Program Shader is linked successfully"); }
	

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	ShaderMap[Default] = programShader;
}

unsigned int ShaderManager::getShader(MaterialType Material) {
	if (ShaderMap.find(Material) != ShaderMap.end()) {
		return ShaderMap[Material];
	}
	else {
		return ShaderMap[Default];
	}
}