#include "AssetSystem.h"

#define STB_IMAGE_IMPLEMENTATION 
#include "../dependencies/stb_image.h"

#include <chrono>
#include <cstdint>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

void AssetSystem::RunTEX() {
    const fs::path objectsDir = fs::path("src") / "Textures";
    const fs::path binsDir = fs::path("src") / "bins";

    if (!fs::exists(objectsDir)) {
        LogError("Objects directory not found: " + objectsDir.string());
        return;
    }

    try {
        for (const auto& entry : fs::directory_iterator(objectsDir)) {
            const std::string ext = entry.path().extension().string();
            if (!entry.is_regular_file() || (ext != ".png" && ext != ".jpg" && ext != ".jpeg"))
                continue;

            AssetInfo info;
            if (!LoadMeta(fs::path(entry.path().string() + ".meta"), info))
                continue;

            if (info.path.empty())
                info.path = (objectsDir / entry.path().filename()).generic_string();

            const fs::path binPath = binsDir / (std::to_string(info.id) + ".bin");
            if (fs::is_regular_file(binPath))
                continue;

            TexData tex;
            if (ParseTEX(info.path, tex))
                WriteBinTEX(binPath, tex);
        }
    }
    catch (const fs::filesystem_error& e) {
        LogError("Filesystem error in Run: " + std::string(e.what()));
    }
}

// Register image files(textures)

void AssetSystem::RegisterFileTEX(const std::string& path) {
    const fs::path src(path);

    const std::string ext = src.extension().string();
    if (!fs::is_regular_file(src) || (ext != ".jpg" && ext != ".png" && ext != ".jpeg")) {
        LogError("Expected an existing .png/.jpg file: " + path);
        return;
    }

    const fs::path objectsDir = fs::path("src") / "Textures";
    const fs::path dest = objectsDir / src.filename();

    try {
        fs::create_directories(objectsDir);
        fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
    }
    catch (const fs::filesystem_error& e) {
        LogError("Failed to register file: " + std::string(e.what()));
        return;
    }

    MakeFileMetaTEX(dest);
    Logger::addLog(LOG_INFO,
        "Texture registered: " + dest.string());
    // We must recheck all textures again and add bin for it
    RunTEX();
}

// Writing information(id and relative path) to .meta file for texture file

void AssetSystem::MakeFileMetaTEX(const fs::path& texPath) {
    const auto   now = std::chrono::system_clock::now();
    const time_t t = std::chrono::system_clock::to_time_t(now);
    tm lt{};

#ifdef _WIN32
    const bool timeOk = (localtime_s(&lt, &t) == 0);
#else
    const bool timeOk = (localtime_r(&t, &lt) != nullptr);
#endif
    if (!timeOk) {
        LogError("Failed to get local time for: " + texPath.string());
        return;
    }

    const uint64_t id =
        static_cast<uint64_t>(lt.tm_year + 1900) * 10000000000ULL +
        static_cast<uint64_t>(lt.tm_mon + 1) * 100000000ULL +
        static_cast<uint64_t>(lt.tm_mday) * 1000000ULL +
        static_cast<uint64_t>(lt.tm_hour) * 10000ULL +
        static_cast<uint64_t>(lt.tm_min) * 100ULL +
        static_cast<uint64_t>(lt.tm_sec);

    const std::string relPath =
        (fs::path("src") / "Textures" / texPath.filename()).generic_string();

    std::ofstream meta(fs::path(texPath.string() + ".meta"), std::ios::trunc);
    if (!meta) { LogError("Failed to create meta for: " + texPath.string()); return; }

    meta << "ID=" << id << '\n'
        << "Path=" << relPath << '\n';

    if (!meta.good()) LogError("Failed to write meta for: " + texPath.string());
}


// Parsing .png/.jpg file

bool AssetSystem::ParseTEX(const fs::path& texPath, TexData& out) {
    int width, height, nrChannels;

    unsigned char* data = stbi_load(texPath.string().c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
        LogError("Failed to load texture: " + texPath.string());
        return false;
    }

    out.width = width;
    out.height = height;
    out.nrChannels = nrChannels;

    const size_t dataSize =
        static_cast<size_t>(width) *
        static_cast<size_t>(height) *
        static_cast<size_t>(nrChannels);

    out.data.assign(data, data + dataSize);

    stbi_image_free(data);
    return true;
}

// Writing texture binary file

bool AssetSystem::WriteBinTEX(const std::filesystem::path& binPath, const TexData& tex) {
    std::ofstream file(binPath, std::ios::binary);
    if (!file) {
        LogError("Cannot open binary file for writing: " + binPath.string());
        return false;
    }

    // Write header (width, height, channels)
    file.write(reinterpret_cast<const char*>(&tex.width), sizeof(tex.width));
    file.write(reinterpret_cast<const char*>(&tex.height), sizeof(tex.height));
    file.write(reinterpret_cast<const char*>(&tex.nrChannels), sizeof(tex.nrChannels));

    // Write data
    if (!tex.data.empty()) {
        file.write(reinterpret_cast<const char*>(tex.data.data()), tex.data.size());
    }

    if (!file.good()) {
        LogError("Failed to write texture binary: " + binPath.string());
        return false;
    }

    return true;
}