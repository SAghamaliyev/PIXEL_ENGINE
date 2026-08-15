#include "Shader.h"
namespace fs = std::filesystem;

ShaderManager::ShaderManager() {
	const char* vertexShaderSource;
	const char* fragmentShaderSource;

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
				vertexShaderSource = finalLine.c_str();
			}
			else if (ItsPath.extension() == ".frag") {
				fragmentShaderSource = finalLine.c_str();
			}		
		}
	}

	// Making Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int programShader = glCreateProgram();
	glAttachShader(programShader, vertexShader);
	glAttachShader(programShader, fragmentShader);
	glLinkProgram(programShader);

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
