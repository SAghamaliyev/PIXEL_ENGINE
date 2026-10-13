#include "AssetSystem.h"

#include <chrono>
#include <cstdint>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Core/Logger/Logger.h"

using namespace std;

// Scans Textures directory for texture files and generates binary cache if missing
void AssetSystem::runTexture() {
    const filesystem::path textures_dir = filesystem::path("assets") / "Textures";
    const filesystem::path bins_dir = filesystem::path("assets") / "bins";

    if (!filesystem::exists(textures_dir)) {
        Logger::addLog(LogType::LOG_ERROR, "Textures directory not found: " + textures_dir.string());
        return;
    }

    try {
        for (const auto& entry : filesystem::directory_iterator(textures_dir)) {
            const string extension = entry.path().extension().string();
            if (!entry.is_regular_file()
                || (extension != ".png"
                    && extension != ".jpg"
                    && extension != ".jpeg")) {
                continue;
            }

            AssetInfo info;
            if (!loadMeta(entry.path().string() + ".meta", info)) {
                continue;
            }

            if (info.path.empty()) {
                info.path = (textures_dir / entry.path().filename()).generic_string();
            }

            const filesystem::path bin_path = bins_dir / (to_string(info.id) + ".bin");
            if (filesystem::is_regular_file(bin_path)) {
                continue;
            }

            TextureAssetData texture;
            if (parseTexture(info.path, texture)) {
                writeTextureBin(bin_path, texture);
            }
        }
    }
    catch (const filesystem::filesystem_error& error) {
        Logger::addLog(LogType::LOG_ERROR, "Filesystem error in runTexture: " + string(error.what()));
    }
}

// Registers a texture file by copying it to Textures folder and creating metadata
void AssetSystem::registerTextureFile(const string& path) {
    const filesystem::path src_path(path);

    const string extension = src_path.extension().string();
    if (!filesystem::is_regular_file(src_path)
        || (extension != ".jpg"
            && extension != ".png"
            && extension != ".jpeg")) {
        Logger::addLog(LogType::LOG_ERROR, "Expected an existing texture file (.png, .jpg, .jpeg): " + path);
        return;
    }

    const filesystem::path textures_dir = filesystem::path("src") / "Textures";
    const filesystem::path dest_path = textures_dir / src_path.filename();

    try {
        filesystem::create_directories(textures_dir);
        filesystem::copy_file(src_path, dest_path, filesystem::copy_options::overwrite_existing);
    }
    catch (const filesystem::filesystem_error& error) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to register texture file: " + string(error.what()));
        return;
    }

    makeTextureMeta(dest_path);
    Logger::addLog(
        LogType::LOG_INFO,
        "Texture registered: " + dest_path.string());

    // Recheck all textures and generate binary cache
    runTexture();
}

// Generates .meta file with unique ID and path for a texture asset
void AssetSystem::makeTextureMeta(const filesystem::path& texturePath) {
    const auto now = chrono::system_clock::now();
    const time_t current_time = chrono::system_clock::to_time_t(now);
    tm local_time{};

#ifdef _WIN32
    const bool time_ok = (localtime_s(&local_time, &current_time) == 0);
#else
    const bool time_ok = (localtime_r(&current_time, &local_time) != nullptr);
#endif
    if (!time_ok) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to get local time for: " + texturePath.string());
        return;
    }

    const uint64_t id =
        static_cast<uint64_t>(local_time.tm_year + 1900) * 10000000000ULL +
        static_cast<uint64_t>(local_time.tm_mon + 1)      *   100000000ULL +
        static_cast<uint64_t>(local_time.tm_mday)         *     1000000ULL +
        static_cast<uint64_t>(local_time.tm_hour)         *       10000ULL +
        static_cast<uint64_t>(local_time.tm_min)          *         100ULL +
        static_cast<uint64_t>(local_time.tm_sec);

    const string rel_path =
        (filesystem::path("src") / "Textures" / texturePath.filename()).generic_string();

    ofstream meta_file(texturePath.string() + ".meta", ios::trunc);
    if (!meta_file.is_open()) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to create meta for: " + texturePath.string());
        return;
    }

    meta_file << "ID=" << id << '\n'
              << "Path=" << rel_path << '\n';

    if (!meta_file.good()) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to write meta for: " + texturePath.string());
    }
}

// Reads a texture file and takes data from it
bool AssetSystem::parseTexture(const filesystem::path& texturePath, TextureAssetData& outTexture) {
    int width = 0;
    int height = 0;
    int channel_count = 0;

    unsigned char* p_data = stbi_load(texturePath.string().c_str(), &width, &height, &channel_count, 0);
    if (p_data == nullptr) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to load texture: " + texturePath.string());
        return false;
    }

    outTexture.width = width;
    outTexture.height = height;
    outTexture.nrChannels = channel_count;

    const size_t data_size =
        static_cast<size_t>(width) *
        static_cast<size_t>(height) *
        static_cast<size_t>(channel_count);

    outTexture.data.assign(p_data, p_data + data_size);

    stbi_image_free(p_data);
    return true;
}

// Writes parsed texture data to binary cache file for fast loading
bool AssetSystem::writeTextureBin(const filesystem::path& binPath, const TextureAssetData& texture) {
    try {
        filesystem::create_directories(binPath.parent_path());
    }
    catch (const filesystem::filesystem_error& error) {
        Logger::addLog(LogType::LOG_ERROR, "Cannot create bins directory: " + string(error.what()));
        return false;
    }

    ofstream bin_file(binPath, ios::binary | ios::trunc);
    if (!bin_file.is_open()) {
        Logger::addLog(LogType::LOG_ERROR, "Cannot open binary file for writing: " + binPath.string());
        return false;
    }

    // Write header (width, height, channels)
    bin_file.write(reinterpret_cast<const char*>(&texture.width), sizeof(texture.width));
    bin_file.write(reinterpret_cast<const char*>(&texture.height), sizeof(texture.height));
    bin_file.write(reinterpret_cast<const char*>(&texture.nrChannels), sizeof(texture.nrChannels));

    // Write pixel buffer
    if (!texture.data.empty()) {
        bin_file.write(reinterpret_cast<const char*>(texture.data.data()), texture.data.size());
    }

    if (!bin_file.good()) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to write texture binary file: " + binPath.string());
        return false;
    }

    return true;
}
