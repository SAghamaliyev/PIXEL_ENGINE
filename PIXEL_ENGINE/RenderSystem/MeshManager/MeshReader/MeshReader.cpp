#include "MeshReader.h"

void ReaderOBJ::IndicesParse(const string& line, vector <int> &indices) {
    istringstream iss(line);
    string type;
    iss >> type; // "f", отбрасываем

    string token;
    while (iss >> token) {
        // token: "12/1/1", "12//1" или просто "12" — берём часть до первого '/'
        size_t PositionSlash = token.find('/');
        string OurNumber = (PositionSlash == string::npos) ? token : token.substr(0, PositionSlash);
        if (!OurNumber.empty()) {
            indices.push_back(stof(OurNumber));
        }
    }
    return;
}

void ReaderOBJ::ObjectReader(const string& path, vector <float> &vertices, vector <int> &indices) {
    ifstream file(path);

    if (!file.is_open()) {
        throw std::runtime_error("Can't open shader in ReaderOBJ ");
        return;
    }

    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        string type;
        float x, y, z;

        if (iss >> type) {
            if (type == "v") {
                if (iss >> x >> y >> z) {
                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);
                }
            }
            else if (type == "f") {
                IndicesParse(line, indices);
            }
        }
    }

    return;
}

