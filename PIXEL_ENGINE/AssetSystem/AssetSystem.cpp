#include "AssetSystem.h"


#include <chrono>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

void AssetSystem::LogError(const std::string& msg) {
    Logger::addLog(LOG_ERROR, msg);
}

// Check if .meta file is okay
bool AssetSystem::LoadMeta(const fs::path& metaPath, AssetInfo& out) {
    std::ifstream file(metaPath);
    if (!file) { LogError("Cannot open meta file: " + metaPath.string()); return false; }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        const auto sep = line.find('=');
        if (sep == std::string::npos) continue;

        const std::string key = line.substr(0, sep);
        const std::string value = line.substr(sep + 1);

        if (key == "ID") {
            try { out.id = std::stoull(value); }
            catch (...) { LogError("Invalid ID in meta: " + metaPath.string()); return false; }
        }
        else if (key == "Path") {
            out.path = value;
        }
    }

    if (out.id == 0) { LogError("Missing or zero ID in meta: " + metaPath.string()); return false; }
    return true;
}


void AssetSystem::Run() {
    RunOBJ();
    RunTEX();
}

void AssetSystem::RegisterFile(const std::string& path) {
    const fs::path src(path);
    const std::string ext = src.extension().string();

    if (ext == ".obj")
        RegisterFileOBJ(path);
    else if (ext == ".png" || ext == ".jpg" || ext == ".jpeg")
        RegisterFileTEX(path);
    else
        LogError("Unsupported file type: " + path);
}

