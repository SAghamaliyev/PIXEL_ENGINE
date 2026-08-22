#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
using namespace std;

enum MaterialType {
    Default = 0,   
    Unlit,       
    PBR_Standard,  
    BlinnPhong,      
    Glass,           
    Water,           
    Terrain,         
    Skybox,          
    Custom           
};

struct MeshInfo {
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;
	unsigned int indexcount = 0;
};

struct EntityUnit {
	unsigned int EntityID;
	MaterialType MaterialID;

	string path;
	string name;
	bool isActive;
};

class EntityManager {
private:
	unsigned int counter;
	unordered_map <unsigned int, EntityUnit> EntityList;
	vector <unsigned int> DeactivatedList;
public:
	EntityManager();

	void createEntity(const string& Path, MaterialType Type, const string& Name = "Object");
	void deleteEntity(unsigned int EntityID);
	void Update();

	const unordered_map <unsigned int, EntityUnit>& getEntityList();
	EntityUnit& getEntity(unsigned int EntityID);

};