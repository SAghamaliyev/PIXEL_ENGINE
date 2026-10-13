#include "AssetSystem.h"

#include <chrono>
#include <cstdint>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Core/Logger/Logger.h"

using namespace std;

// Scans Objects directory for mesh files and generates binary cache if missing
void AssetSystem::runMesh() {
    const filesystem::path meshes_dir = filesystem::path("assets") / "Objects";
    const filesystem::path bins_dir = filesystem::path("assets") / "bins";

    if (!filesystem::exists(meshes_dir)) {
        Logger::addLog(LogType::LOG_ERROR, "Meshes directory not found: " + meshes_dir.string());
        return;
    }

    try {
        for (const auto& entry : filesystem::directory_iterator(meshes_dir)) {
            if (!entry.is_regular_file() || entry.path().extension() != ".obj") {
                continue;
            }

            AssetInfo info;
            if (!loadMeta(entry.path().string() + ".meta", info)) {
                continue;
            }

            if (info.path.empty()) {
                info.path = (meshes_dir / entry.path().filename()).generic_string();
            }

            const filesystem::path bin_path = bins_dir / (to_string(info.id) + ".bin");
            if (filesystem::is_regular_file(bin_path)) {
                continue;
            }

            MeshAssetData mesh;
            if (parseMesh(info.path, mesh)) {
                writeMeshBin(bin_path, mesh);
            }
        }
    }
    catch (const filesystem::filesystem_error& error) {
        Logger::addLog(LogType::LOG_ERROR, "Filesystem error in runMesh: " + string(error.what()));
    }
}

// Registers a mesh file by copying it to Objects folder and creating metadata
void AssetSystem::registerMeshFile(const string& path) {
    const filesystem::path src_path(path);

    if (!filesystem::is_regular_file(src_path) || src_path.extension() != ".obj") {
        Logger::addLog(LogType::LOG_ERROR, "Expected an existing .obj file: " + path);
        return;
    }

    const filesystem::path meshes_dir = filesystem::path("src") / "Objects";
    const filesystem::path dest_path = meshes_dir / src_path.filename();

    try {
        filesystem::create_directories(meshes_dir);
        filesystem::copy_file(src_path, dest_path, filesystem::copy_options::overwrite_existing);
    }
    catch (const filesystem::filesystem_error& error) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to register mesh file: " + string(error.what()));
        return;
    }

    makeMeshMeta(dest_path);
    Logger::addLog(
        LogType::LOG_INFO,
        "Mesh registered: " + dest_path.string());

    // Recheck all mesh files and generate binary cache
    runMesh();
}

// Generates .meta file with ID and path for a mesh asset
void AssetSystem::makeMeshMeta(const filesystem::path& meshPath) {
    const auto now = chrono::system_clock::now();
    const time_t current_time = chrono::system_clock::to_time_t(now);
    tm local_time{};

#ifdef _WIN32
    const bool time_ok = (localtime_s(&local_time, &current_time) == 0);
#else
    const bool time_ok = (localtime_r(&current_time, &local_time) != nullptr);
#endif
    if (!time_ok) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to get local time for: " + meshPath.string());
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
        (filesystem::path("src") / "Objects" / meshPath.filename()).generic_string();

    ofstream meta_file(meshPath.string() + ".meta", ios::trunc);
    if (!meta_file.is_open()) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to create meta for: " + meshPath.string());
        return;
    }

    meta_file << "ID=" << id << '\n'
              << "Path=" << rel_path << '\n';

    if (!meta_file.good()) {
        Logger::addLog(LogType::LOG_ERROR, "Failed to write meta for: " + meshPath.string());
    }
}

// Parses a .obj file and extracts vertices, indices, and texture coordinates
bool AssetSystem::parseMesh(const filesystem::path& meshPath, MeshAssetData& outMesh) {
    ifstream file(meshPath);
    if (!file.is_open()) {
        Logger::addLog(LogType::LOG_ERROR, "Cannot open mesh file: " + meshPath.string());
        return false;
    }

    vector<float> positions;
    vector<float> texcoords;

    struct Corner {
        int v = 0;
        int vt = 0;
    };

    auto emit_corner = [&](const Corner& corner) -> bool {
        int vertex_index = corner.v;
        int tex_index = corner.vt;

        const int position_count = static_cast<int>(positions.size() / 3);
        const int tex_count = static_cast<int>(texcoords.size() / 2);

        if (vertex_index < 0) {
            vertex_index = position_count + vertex_index + 1;
        }
        if (tex_index < 0) {
            tex_index = tex_count + tex_index + 1;
        }

        const int vi = vertex_index - 1;
        if (vi < 0 || vi * 3 + 2 >= static_cast<int>(positions.size())) {
            Logger::addLog(LogType::LOG_ERROR, "Face vertex out of range in mesh: " + meshPath.string());
            return false;
        }

        const float x = positions[vi * 3 + 0];
        const float y = positions[vi * 3 + 1];
        const float z = positions[vi * 3 + 2];
        outMesh.vertices.insert(outMesh.vertices.end(), { x, y, z });

        if (tex_index > 0 && (tex_index - 1) * 2 + 1 < static_cast<int>(texcoords.size())) {
            outMesh.textures.push_back(texcoords[(tex_index - 1) * 2 + 0]);
            // Invert V: OBJ V=0 is bottom, OpenGL texture V=0 is top
            outMesh.textures.push_back(1.0f - texcoords[(tex_index - 1) * 2 + 1]);
        }
        else {
            outMesh.textures.push_back(x * 0.5f + 0.5f);
            outMesh.textures.push_back(y * 0.5f + 0.5f);
        }

        return true;
    };

    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        istringstream stream(line);
        string token;
        stream >> token;

        if (token == "v") {
            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;
            if (!(stream >> x >> y >> z)) {
                Logger::addLog(LogType::LOG_ERROR, "Malformed vertex in mesh: " + meshPath.string());
                return false;
            }
            positions.insert(positions.end(), { x, y, z });
        }
        else if (token == "vt") {
            float u = 0.0f;
            float v = 0.0f;
            if (!(stream >> u >> v)) {
                Logger::addLog(LogType::LOG_ERROR, "Malformed texcoord in mesh: " + meshPath.string());
                return false;
            }
            texcoords.insert(texcoords.end(), { u, v });
        }
        else if (token == "f") {
            vector<Corner> face;
            string face_token;
            while (stream >> face_token) {
                Corner corner;
                try {
                    const auto first_slash = face_token.find('/');
                    if (first_slash == string::npos) {
                        corner.v = stoi(face_token);
                    }
                    else {
                        corner.v = stoi(face_token.substr(0, first_slash));
                        const auto second_slash = face_token.find('/', first_slash + 1);
                        const string vt_part = (second_slash == string::npos)
                            ? face_token.substr(first_slash + 1)
                            : face_token.substr(first_slash + 1, second_slash - first_slash - 1);
                        if (!vt_part.empty()) {
                            corner.vt = stoi(vt_part);
                        }
                    }
                }
                catch (...) {
                    Logger::addLog(LogType::LOG_ERROR, "Invalid face index in mesh: " + meshPath.string());
                    return false;
                }
                face.push_back(corner);
            }

            if (face.size() < 3) {
                continue;
            }

            for (size_t i = 1; i + 1 < face.size(); ++i) {
                const unsigned int base = static_cast<unsigned int>(outMesh.vertices.size() / 3);
                if (!emit_corner(face[0])
                    || !emit_corner(face[i])
                    || !emit_corner(face[i + 1])) {
                    return false;
                }
                outMesh.indices.push_back(base);
                outMesh.indices.push_back(base + 1);
                outMesh.indices.push_back(base + 2);
            }
        }
    }

    return !outMesh.vertices.empty() && !outMesh.indices.empty();
}

// Writes parsed mesh data to binary cache file for fast loading
bool AssetSystem::writeMeshBin(const filesystem::path& binPath, const MeshAssetData& mesh) {
    if (mesh.vertices.size() > UINT32_MAX
        || mesh.indices.size() > UINT32_MAX
        || mesh.textures.size() > UINT32_MAX) {
        Logger::addLog(LogType::LOG_ERROR, "Mesh too large for binary file: " + binPath.string());
        return false;
    }

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

    const uint32_t vertex_count = static_cast<uint32_t>(mesh.vertices.size());
    const uint32_t index_count = static_cast<uint32_t>(mesh.indices.size());
    const uint32_t texture_count = static_cast<uint32_t>(mesh.textures.size());

    bin_file.write(reinterpret_cast<const char*>(&vertex_count), sizeof(vertex_count));
    bin_file.write(reinterpret_cast<const char*>(&index_count), sizeof(index_count));
    bin_file.write(reinterpret_cast<const char*>(&texture_count), sizeof(texture_count));

    if (!mesh.vertices.empty()) {
        bin_file.write(reinterpret_cast<const char*>(mesh.vertices.data()), vertex_count * sizeof(float));
    }
    if (!mesh.indices.empty()) {
        bin_file.write(reinterpret_cast<const char*>(mesh.indices.data()), index_count * sizeof(unsigned int));
    }
    if (!mesh.textures.empty()) {
        bin_file.write(reinterpret_cast<const char*>(mesh.textures.data()), texture_count * sizeof(float));
    }

    if (!bin_file.good()) {
        Logger::addLog(LogType::LOG_ERROR, "Write error for binary mesh file: " + binPath.string());
        return false;
    }

    return true;
}
