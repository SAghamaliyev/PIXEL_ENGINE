#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class ReaderOBJ {
public:
    static void ObjectReader(const string& path, vector <float> &vertices, vector <int> &indices, vector <float> &textures);  // Read data from .obj files(vertices,indices)
private:
    static void IndicesParse(const string& line, vector <int> &indices);

};
