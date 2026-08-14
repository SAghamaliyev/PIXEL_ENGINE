#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

struct MeshInfo {
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    unsigned int indexcount = 0;
};

class MeshManager {
public:
    MeshManager() = default;
	MeshInfo getMesh(const string& path, const string& name);

    ~MeshManager();
private:
	unordered_map <const string /*Name*/, MeshInfo /*OurMesh*/> MeshMap;
	MeshInfo makeMesh(const string& path);
};