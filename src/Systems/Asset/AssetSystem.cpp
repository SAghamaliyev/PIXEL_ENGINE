#include "AssetSystem.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>

#include "Core/Logger/Logger.h"

using namespace std;

// Loads metadata from a .meta file and populates AssetInfo structure
bool AssetSystem::loadMeta(const filesystem::path& metaPath, AssetInfo& outInfo) {
    ifstream file(metaPath);
    if (!file.is_open()) {
        Logger::addLog(LogType::LOG_ERROR, "Cannot open meta file: " + metaPath.string());
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        const auto sep = line.find('=');
        if (sep == string::npos) {
            continue;
        }

        const string key = line.substr(0, sep);
        const string value = line.substr(sep + 1);

        if (key == "ID") {
            try {
                outInfo.id = stoull(value);
            }
            catch (...) {
                Logger::addLog(LogType::LOG_ERROR, "Invalid ID in meta: " + metaPath.string());
                return false;
            }
        }
        else if (key == "Path") {
            outInfo.path = value;
        }
    }

    if (outInfo.id == 0) {
        Logger::addLog(LogType::LOG_ERROR, "Missing or zero ID in meta: " + metaPath.string());
        return false;
    }

    return true;
}

// Processes all registered asset files and creates/updates their binary representations
void AssetSystem::run() {
    runMesh();
    runTexture();
}

// Registers a new asset file and prepares it for processing
void AssetSystem::registerFile(const string& path) {
    const filesystem::path src_path(path);
    const string extension = src_path.extension().string();

    if (extension == ".obj") {
        registerMeshFile(path);
    }
    else if (extension == ".png"
             || extension == ".jpg"
             || extension == ".jpeg") {
        registerTextureFile(path);
    }
    else {
        Logger::addLog(LogType::LOG_ERROR, "Unsupported file type: " + path);
    }
}
