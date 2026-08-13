#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

using namespace std;

typedef struct Meshinfo {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int indexcount = 0;
}MeshInfo;

class Mesh {
public:
	static MeshInfo getVAO(const string& path);
};