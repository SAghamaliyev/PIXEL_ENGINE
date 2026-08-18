#include <string>

class Object {
private:
	int MeshID;
	int MaterialID;

	string Name;
	string Path;
	static unsigned int counter;
public:
	Object(const string& path);

	// Getters
	MeshInfo getMesh();
	MaterialType getMaterial();
	unsigned int getObjectShader();

	string getName();
	string getPath();
};