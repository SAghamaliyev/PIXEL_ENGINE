#include "AssetSystem.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Core/Logger/Logger.h"

using namespace std;

// Scans project assets directory and compiles missing binaries
void AssetSystem::run() {
    try {
        filesystem::create_directories(filesystem::path("assets") / "bins");
    }
    catch (const filesystem::filesystem_error& e) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to create bins directory: " + string(e.what()));
        return;
    }

    processDirectory(filesystem::path("assets") / "Objects");
    processDirectory(filesystem::path("assets") / "Textures");
}

// Scans a specific directory and processes all assets inside it
void AssetSystem::processDirectory(const filesystem::path& directory) {
    if (!filesystem::exists(directory)) {
        return;
    }

    try {
        for (const auto& entry : filesystem::directory_iterator(directory)) {
            if (!entry.is_regular_file()) {
                continue;
            }
            if (entry.path().extension() == ".meta") {
                continue;
            }

            const uint64_t id = getMeta(entry.path());
            if (id != 0) {
                prepareToCompileAsset(entry.path(), id);
            }
        }
    }
    catch (const filesystem::filesystem_error& e) {
        Logger::addLog(LogType::LOG_ERROR, "Error scanning directory " + directory.string() + ": " + e.what());
    }
}

// Registers a file into the project, creates metadata and compiles binary cache
void AssetSystem::registerFile(const string& path) {
    const filesystem::path src_path(path);
    if (!filesystem::is_regular_file(src_path)) {
        Logger::addLog(LogType::LOG_ERROR, "Asset file does not exist: " + path);
        return;
    }

    const string ext = src_path.extension().string();
    filesystem::path dest_dir;

    if (ext == ".obj") {
        dest_dir = filesystem::path("assets") / "Objects";
    }
    else if (ext == ".png"
             || ext == ".jpg"
             || ext == ".jpeg") {
        dest_dir = filesystem::path("assets") / "Textures";
    }
    else {
        Logger::addLog(LogType::LOG_ERROR, "Unsupported asset extension: " + path);
        return;
    }

    const filesystem::path dest_path = dest_dir / src_path.filename();

    try {
        filesystem::create_directories(dest_dir);
        if (filesystem::absolute(src_path) != filesystem::absolute(dest_path)) {
            filesystem::copy_file(src_path,
                                  dest_path,
                                  filesystem::copy_options::overwrite_existing);
        }
    }
    catch (const filesystem::filesystem_error& e) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to copy file into assets: " + string(e.what()));
        return;
    }

    const uint64_t id = getMeta(dest_path);
    if (id == 0) {
        return;
    }

    prepareToCompileAsset(dest_path, id);
    Logger::addLog(LogType::LOG_INFO, "Asset registered: " + dest_path.string());
}

// Reads ID from .meta file. If meta doesn't exist, calls createMeta() and reads again
uint64_t AssetSystem::getMeta(const filesystem::path& assetPath) {
    const filesystem::path meta_path = assetPath.string() + ".meta";

    // If meta file does not exist, create it first
    if (!filesystem::is_regular_file(meta_path)) {
        createMeta(assetPath);
    }

    ifstream file(meta_path);
    if (!file.is_open()) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to open meta file: " + meta_path.string());
        return 0;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.rfind("ID=", 0) == 0) {
            try {
                return stoull(line.substr(3));
            }
            catch (...) {
                break;
            }
        }
    }

    Logger::addLog(LogType::LOG_ERROR, "Invalid meta data in: " + meta_path.string());
    return 0;
}

// Generates a unique 64-bit ID and writes a new .meta file next to the asset
void AssetSystem::createMeta(const filesystem::path& assetPath) {
    const filesystem::path meta_path = assetPath.string() + ".meta";

    // Fast non-zero 64-bit ID generation with uniform distribution
    static mt19937_64 rng(random_device{}());
    uint64_t id = rng();
    while (id == 0) {
        id = rng();
    }

    ofstream meta_file(meta_path, ios::trunc);
    if (!meta_file.is_open()) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to create meta file: " + meta_path.string());
        return;
    }

    meta_file << "ID=" << id << '\n'
              << "Path=" << assetPath.generic_string() << '\n';

    if (!meta_file.good()) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to write meta data to: " + meta_path.string());
    }
}

// Checks if <id>.bin exists and is fresh. If missing or outdated, triggers compilation
void AssetSystem::prepareToCompileAsset(const filesystem::path& assetPath, uint64_t id) {
    if (id == 0) {
        Logger::addLog(LogType::LOG_ERROR, "Cannot compile asset without valid ID: " + assetPath.string());
        return;
    }

    const filesystem::path bin_path = filesystem::path("assets") / "bins" / (to_string(id) + ".bin");

    // Check if binary cache exists and is up to date with the source file
    error_code ec;
    if (filesystem::exists(bin_path, ec)) {
        const auto bin_time = filesystem::last_write_time(bin_path, ec);
        const auto src_time = filesystem::last_write_time(assetPath, ec);
        if (!ec && bin_time >= src_time) {
            return; // Cache is valid and fresh, no need to recompile
        }
    }

    const string ext = assetPath.extension().string();
    if (ext == ".obj") {
        compileMesh(assetPath, bin_path);
    }
    else if (ext == ".png"
             || ext == ".jpg"
             || ext == ".jpeg") {
        compileTexture(assetPath, bin_path);
    }
    else {
        Logger::addLog(LogType::LOG_ERROR, "Unsupported asset type: " + assetPath.string());
    }
}


// Parses an OBJ mesh and writes a deduplicated binary cache to bin_path
void AssetSystem::compileMesh(const filesystem::path& srcPath, const filesystem::path& binPath) {
    ifstream file(srcPath);
    if (!file.is_open()) {
        Logger::addLog(LogType::LOG_ERROR, "Cannot open mesh file: " + srcPath.string());
        return;
    }

    // Raw data read from the OBJ file
    vector<float> raw_positions;    // v:  x, y, z
    vector<float> raw_texcoords;    // vt: u, v

    // Final data for the binary file
    vector<float> positions;        // x, y, z per unique vertex
    vector<float> texcoords;        // u, v per unique vertex, parallel to positions
    vector<unsigned int> indices;   // three indices per triangle

    // Key: (position index << 32) | texcoord index  ->  unique vertex index
    unordered_map<uint64_t, unsigned int> vertex_lookup;

    constexpr uint32_t no_texcoord = 0xFFFFFFFFu;

    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        istringstream stream(line);
        string token;
        stream >> token;

        // Vertex position: v X Y Z
        if (token == "v") {
            float x = 0.0f, y = 0.0f, z = 0.0f;
            if (!(stream >> x >> y >> z)) {
                Logger::addLog(LogType::LOG_ERROR, "Malformed vertex line in mesh: " + srcPath.string());
                return;
            }

            raw_positions.push_back(x);
            raw_positions.push_back(y);
            raw_positions.push_back(z);
        }
        // Texture coordinate: vt U V
        else if (token == "vt") {
            float u = 0.0f, v = 0.0f;
            if (!(stream >> u >> v)) {
                Logger::addLog(LogType::LOG_ERROR, "Malformed texcoord line in mesh: " + srcPath.string());
                return;
            }

            raw_texcoords.push_back(u);
            raw_texcoords.push_back(v);
        }
        // Face: f v1/vt1 v2/vt2 v3/vt3
        else if (token == "f") {
            vector<int> face_position_indices;
            vector<int> face_texcoord_indices;

            string face_token;
            while (stream >> face_token) {
                int position_index = 0;
                int texcoord_index = 0;

                try {
                    const auto first_slash = face_token.find('/');

                    if (first_slash == string::npos) {
                        position_index = stoi(face_token);
                    }
                    else {
                        position_index = stoi(face_token.substr(0, first_slash));

                        const auto second_slash = face_token.find('/', first_slash + 1);
                        const string texcoord_part = (second_slash == string::npos)
                            ? face_token.substr(first_slash + 1)
                            : face_token.substr(first_slash + 1, second_slash - first_slash - 1);

                        if (!texcoord_part.empty()) {
                            texcoord_index = stoi(texcoord_part);
                        }
                    }
                }
                catch (...) {
                    Logger::addLog(LogType::LOG_ERROR, "Invalid face index in mesh: " + srcPath.string());
                    return;
                }

                face_position_indices.push_back(position_index);
                face_texcoord_indices.push_back(texcoord_index);
            }

            if (face_position_indices.size() < 3) {
                continue;
            }

            const int position_count = static_cast<int>(raw_positions.size() / 3);
            const int texcoord_count = static_cast<int>(raw_texcoords.size() / 2);

            // Fan triangulation: (0, i, i + 1)
            for (size_t i = 1; i + 1 < face_position_indices.size(); ++i) {
                const size_t triangle_corners[] = {0, i, i + 1};
                unsigned int triangle_indices[3] = {0, 0, 0};

                for (size_t corner = 0; corner < 3; ++corner) {
                    const size_t face_corner = triangle_corners[corner];
                    int position_index = face_position_indices[face_corner];
                    int texcoord_index = face_texcoord_indices[face_corner];

                    // Negative OBJ indices are relative to the end of the list
                    if (position_index < 0) {
                        position_index = position_count + position_index + 1;
                    }
                    if (texcoord_index < 0) {
                        texcoord_index = texcoord_count + texcoord_index + 1;
                    }

                    // OBJ indices are 1-based, convert to 0-based
                    const int raw_position_index = position_index - 1;
                    if (raw_position_index < 0 || raw_position_index >= position_count) {
                        Logger::addLog(LogType::LOG_ERROR, "Face vertex out of range in mesh: " + srcPath.string());
                        return;
                    }

                    const int raw_texcoord_index = texcoord_index - 1;
                    const bool has_texcoord = raw_texcoord_index >= 0 && raw_texcoord_index < texcoord_count;

                    const uint32_t texcoord_key = has_texcoord
                        ? static_cast<uint32_t>(raw_texcoord_index)
                        : no_texcoord;
                    const uint64_t vertex_key =
                        (static_cast<uint64_t>(raw_position_index) << 32) | texcoord_key;

                    // Reuse an existing vertex or create a new one
                    const auto [lookup_it, inserted] = vertex_lookup.try_emplace(
                        vertex_key, static_cast<unsigned int>(positions.size() / 3));

                    if (inserted) {
                        const float x = raw_positions[raw_position_index * 3 + 0];
                        const float y = raw_positions[raw_position_index * 3 + 1];
                        const float z = raw_positions[raw_position_index * 3 + 2];

                        positions.push_back(x);
                        positions.push_back(y);
                        positions.push_back(z);

                        if (has_texcoord) {
                            texcoords.push_back(raw_texcoords[raw_texcoord_index * 2 + 0]);
                            // Invert V: OBJ V=0 is bottom, OpenGL texture origin is top-left
                            texcoords.push_back(1.0f - raw_texcoords[raw_texcoord_index * 2 + 1]);
                        }
                        else {
                            // Fallback UVs derived from position
                            texcoords.push_back(x * 0.5f + 0.5f);
                            texcoords.push_back(y * 0.5f + 0.5f);
                        }
                    }

                    triangle_indices[corner] = lookup_it->second;
                }

                indices.push_back(triangle_indices[0]);
                indices.push_back(triangle_indices[1]);
                indices.push_back(triangle_indices[2]);
            }
        }
    }

    if (positions.empty()) {
        Logger::addLog(LogType::LOG_ERROR, "No vertex data found in mesh: " + srcPath.string());
        return;
    }

    try {
        filesystem::create_directories(binPath.parent_path());
    }
    catch (const filesystem::filesystem_error& e) {
        Logger::addLog(LogType::LOG_ERROR, "Cannot create bins directory: " + string(e.what()));
        return;
    }

    ofstream bin_file(binPath, ios::binary | ios::trunc);
    if (!bin_file.is_open()) {
        Logger::addLog(LogType::LOG_ERROR, "Cannot open binary file for writing: " + binPath.string());
        return;
    }

    const uint32_t position_float_count = static_cast<uint32_t>(positions.size());
    const uint32_t index_count = static_cast<uint32_t>(indices.size());
    const uint32_t texcoord_float_count = static_cast<uint32_t>(texcoords.size());

    bin_file.write(reinterpret_cast<const char*>(&position_float_count), sizeof(position_float_count));
    bin_file.write(reinterpret_cast<const char*>(&index_count), sizeof(index_count));
    bin_file.write(reinterpret_cast<const char*>(&texcoord_float_count), sizeof(texcoord_float_count));

    bin_file.write(reinterpret_cast<const char*>(positions.data()),
                   position_float_count * sizeof(float));
    bin_file.write(reinterpret_cast<const char*>(indices.data()),
                   index_count * sizeof(unsigned int));
    bin_file.write(reinterpret_cast<const char*>(texcoords.data()),
                   texcoord_float_count * sizeof(float));

    if (!bin_file.good()) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to write mesh binary file: " + binPath.string());
    }
}

// Loads texture image (PNG, JPG) and writes raw pixel binary cache directly into binPath
void AssetSystem::compileTexture(const filesystem::path& srcPath, const filesystem::path& binPath) {
    int width = 0;
    int height = 0;
    int channel_count = 0;

    unsigned char* p_data = stbi_load(srcPath.string().c_str(),
                                      &width,
                                      &height,
                                      &channel_count,
                                      0);
    if (p_data == nullptr) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to load texture: " + srcPath.string());
        return;
    }

    try {
        filesystem::create_directories(binPath.parent_path());
    }
    catch (const filesystem::filesystem_error& e) {
        stbi_image_free(p_data);
        Logger::addLog(LogType::LOG_ERROR, "Cannot create bins directory: " + string(e.what()));
        return;
    }

    ofstream bin_file(binPath, ios::binary | ios::trunc);
    if (!bin_file.is_open()) {
        stbi_image_free(p_data);
        Logger::addLog(LogType::LOG_ERROR, "Cannot open binary file for writing: " + binPath.string());
        return;
    }

    bin_file.write(reinterpret_cast<const char*>(&width), sizeof(width));
    bin_file.write(reinterpret_cast<const char*>(&height), sizeof(height));
    bin_file.write(reinterpret_cast<const char*>(&channel_count), sizeof(channel_count));

    const size_t data_size = static_cast<size_t>(width)
                           * static_cast<size_t>(height)
                           * static_cast<size_t>(channel_count);
    if (data_size > 0) {
        bin_file.write(reinterpret_cast<const char*>(p_data), data_size);
    }

    stbi_image_free(p_data);

    if (!bin_file.good()) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to write texture binary file: " + binPath.string());
    }
}
