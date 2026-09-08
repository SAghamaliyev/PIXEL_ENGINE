#include "AssetSystem.h"

#include "AssetSystem.h"

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>

void AssetSystem::Run() {
    PathRegistry.clear();

    const filesystem::path objectsDirectory = filesystem::path("src") / "objects";
    const filesystem::path binsDirectory = filesystem::path("src") / "bins";

    if (!filesystem::exists(objectsDirectory)) {
        return;
    }

    for (const auto& objectEntry : filesystem::directory_iterator(objectsDirectory)) {
        if (!objectEntry.is_regular_file() || objectEntry.path().extension() != ".obj") {
            continue;
        }

        const filesystem::path metaPath = objectEntry.path().string() + ".meta";
        ifstream metaFile(metaPath);
        if (!metaFile.is_open()) {
            Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
                "Meta file not found for object: " + objectEntry.path().string());
            continue;
        }

        unsigned long long int id = 0;
        string relativePath;
        string line;

        while (getline(metaFile, line)) {
            const size_t separator = line.find('=');
            if (separator == string::npos) {
                continue;
            }

            const string key = line.substr(0, separator);
            const string value = line.substr(separator + 1);

            if (key == "ID") {
                try {
                    size_t parsedCharacters = 0;
                    id = stoull(value, &parsedCharacters);
                    if (parsedCharacters != value.size()) {
                        throw invalid_argument("ID contains non-numeric characters");
                    }
                }
                catch (const invalid_argument&) {
                    id = 0;
                }
                catch (const out_of_range&) {
                    id = 0;
                }
            }
            else if (key == "Path") {
                relativePath = value;
            }
        }

        if (id == 0) {
            Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
                "Invalid ID in meta file: " + metaPath.string());
            continue;
        }

        if (relativePath.empty()) {
            relativePath = (objectsDirectory / objectEntry.path().filename()).generic_string();
        }

        const filesystem::path binaryPath = binsDirectory / (to_string(id) + ".bin");
        if (!filesystem::is_regular_file(binaryPath)) {
            ParseFile(id, relativePath);
        }

        PathRegistry[id] = relativePath;
    }
}

void AssetSystem::ParseFile(const unsigned long long int ID,const string relativePath) {
    ifstream objectFile(relativePath);
    if (!objectFile.is_open()) {
        Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
            "Failed to open object for parsing: " + relativePath);
        return;
    }

    vector<float> vertices;
    vector<unsigned int> indices;
    vector<float> textures;
    string line;

    while (getline(objectFile, line)) {
        istringstream lineStream(line);
        string type;
        lineStream >> type;

        if (type == "v") {
            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;
            if (lineStream >> x >> y >> z) {
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }
        else if (type == "vt") {
            float u = 0.0f;
            float v = 0.0f;
            if (lineStream >> u >> v) {
                textures.push_back(u);
                textures.push_back(v);
            }
        }
        else if (type == "f") {
            string token;
            while (lineStream >> token) {
                const size_t slashPosition = token.find('/');
                const string vertexNumber = slashPosition == string::npos
                    ? token
                    : token.substr(0, slashPosition);

                if (!vertexNumber.empty()) {
                    try {
                        indices.push_back(static_cast<unsigned int>(stoul(vertexNumber)));
                    }
                    catch (const invalid_argument&) {
                        Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
                            "Invalid face index in object: " + relativePath);
                        return;
                    }
                    catch (const out_of_range&) {
                        Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
                            "Face index is out of range in object: " + relativePath);
                        return;
                    }
                }
            }
        }
    }

    const filesystem::path binsDirectory = filesystem::path("src") / "bins";
    const filesystem::path binPath = binsDirectory / (to_string(ID) + ".bin");

    try {
        filesystem::create_directories(binsDirectory);
        ofstream binFile(binPath, ios::binary | ios::trunc);
        if (!binFile.is_open()) {
            Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
                "Failed to create binary asset file: " + binPath.string());
            return;
        }

        const uint32_t verticesCount = static_cast<uint32_t>(vertices.size());
        const uint32_t indicesCount = static_cast<uint32_t>(indices.size());
        const uint32_t texturesCount = static_cast<uint32_t>(textures.size());

        binFile.write(reinterpret_cast<const char*>(&verticesCount), sizeof(verticesCount));
        binFile.write(reinterpret_cast<const char*>(&indicesCount), sizeof(indicesCount));
        binFile.write(reinterpret_cast<const char*>(&texturesCount), sizeof(texturesCount));
        binFile.write(reinterpret_cast<const char*>(vertices.data()),
            static_cast<streamsize>(vertices.size() * sizeof(float)));
        binFile.write(reinterpret_cast<const char*>(indices.data()),
            static_cast<streamsize>(indices.size() * sizeof(unsigned int)));
        binFile.write(reinterpret_cast<const char*>(textures.data()),
            static_cast<streamsize>(textures.size() * sizeof(float)));

        if (!binFile.good()) {
            Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
                "Failed to write binary asset file: " + binPath.string());
            return;
        }
    }
    catch (const filesystem::filesystem_error& error) {
        Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
            "Failed to create binary asset file: " + string(error.what()));
        return;
    }
}

void AssetSystem::MakeFileMeta(string path) {
    const auto now = chrono::system_clock::now();
    const time_t currentTime = chrono::system_clock::to_time_t(now);
    tm localTime{};

#ifdef _WIN32
    if (localtime_s(&localTime, &currentTime) != 0) {
        Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
            "Failed to convert registration time for: " + path);
        return;
    }
#else
    if (localtime_r(&currentTime, &localTime) == nullptr) {
        Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
            "Failed to convert registration time for: " + path);
        return;
    }
#endif

    const unsigned long long int id =
        static_cast<unsigned long long int>(localTime.tm_year + 1900) * 10000000000ULL +
        static_cast<unsigned long long int>(localTime.tm_mon + 1) * 100000000ULL +
        static_cast<unsigned long long int>(localTime.tm_mday) * 1000000ULL +
        static_cast<unsigned long long int>(localTime.tm_hour) * 10000ULL +
        static_cast<unsigned long long int>(localTime.tm_min) * 100ULL +
        static_cast<unsigned long long int>(localTime.tm_sec);

    if (id == 0) {
        Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
            "Generated asset ID is invalid for: " + path);
        return;
    }

    const string relativePath = (filesystem::path("src") / "objects" /
        filesystem::path(path).filename()).generic_string();

    ofstream metaFile(path + ".meta", ios::trunc);
    if (!metaFile.is_open()) {
        Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
            "Failed to create meta file for: " + path);
        return;
    }

    metaFile << "ID=" << id << '\n';
    metaFile << "Path=" << relativePath << '\n';
    if (!metaFile.good()) {
        Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
            "Failed to write meta file for: " + path);
        return;
    }

    ParseFile(id, relativePath);

}

void AssetSystem::RegisterFile(string path) {
    const filesystem::path sourcePath(path);

    if (!filesystem::is_regular_file(sourcePath) || sourcePath.extension() != ".obj") {
        Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
            "Cannot register file. Expected an existing .obj file: " + path);
        return;
    }

    const filesystem::path objectsDirectory = filesystem::path("src") / "objects";
    const filesystem::path destinationPath = objectsDirectory / sourcePath.filename();

    try {
        filesystem::create_directories(objectsDirectory);
        filesystem::copy_file(sourcePath, destinationPath,
            filesystem::copy_options::overwrite_existing);

        // Creating its Meta File
        MakeFileMeta(destinationPath.string());

        Logger::getInstance().addLog(Logger::LogEntry::LOG_INFO,
            "Object registered: " + destinationPath.string());
    }
    catch (const filesystem::filesystem_error& error) {
        Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
            "Error occured: " + string(error.what()));
        return;
    }

}