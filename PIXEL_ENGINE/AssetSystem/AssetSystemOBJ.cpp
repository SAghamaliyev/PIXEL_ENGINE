#include "AssetSystem.h"

#include <chrono>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

// Running through all object files and check if everything is okay

void AssetSystem::RunOBJ() {
    const fs::path objectsDir = fs::path("src") / "Objects";
    const fs::path binsDir    = fs::path("src") / "bins";

    if (!fs::exists(objectsDir)) {
        LogError("Objects directory not found: " + objectsDir.string());
        return;
    }

    try {
        for (const auto& entry : fs::directory_iterator(objectsDir)) {
            if (!entry.is_regular_file() || entry.path().extension() != ".obj")
                continue;

            AssetInfo info;
            if (!LoadMeta(fs::path(entry.path().string() + ".meta"), info))
                continue;

            if (info.path.empty())
                info.path = (objectsDir / entry.path().filename()).generic_string();

            const fs::path binPath = binsDir / (std::to_string(info.id) + ".bin");
            if (fs::is_regular_file(binPath))
                continue;

            MeshData mesh;
            if (ParseOBJ(info.path, mesh))
                WriteBinOBJ(binPath, mesh);
        }
    }
    catch (const fs::filesystem_error& e) {
        LogError("Filesystem error in Run: " + std::string(e.what()));
    }
}

// Register .obj file

void AssetSystem::RegisterFileOBJ(const std::string& path) {
    const fs::path src(path);

    if (!fs::is_regular_file(src) || src.extension() != ".obj") {
        LogError("Expected an existing .obj file: " + path);
        return;
    }

    const fs::path objectsDir = fs::path("src") / "objects";
    const fs::path dest       = objectsDir / src.filename();

    try {
        fs::create_directories(objectsDir);
        fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
    }
    catch (const fs::filesystem_error& e) {
        LogError("Failed to register file: " + std::string(e.what()));
        return;
    }

    MakeFileMetaOBJ(dest);
    Logger::addLog(LOG_INFO,
        "Object registered: " + dest.string());

    // We must recheck all .obj again and add bin for it
    RunOBJ();
}

// Writing information(id and relative path) to .meta file for .obj file

void AssetSystem::MakeFileMetaOBJ(const fs::path& objPath) {
    const auto   now = std::chrono::system_clock::now();
    const time_t t   = std::chrono::system_clock::to_time_t(now);
    tm lt{};

#ifdef _WIN32
    const bool timeOk = (localtime_s(&lt, &t) == 0);
#else
    const bool timeOk = (localtime_r(&t, &lt) != nullptr);
#endif
    if (!timeOk) {
        LogError("Failed to get local time for: " + objPath.string());
        return;
    }

    const uint64_t id =
        static_cast<uint64_t>(lt.tm_year + 1900) * 10000000000ULL +
        static_cast<uint64_t>(lt.tm_mon  +    1) *   100000000ULL +
        static_cast<uint64_t>(lt.tm_mday)         *     1000000ULL +
        static_cast<uint64_t>(lt.tm_hour)         *       10000ULL +
        static_cast<uint64_t>(lt.tm_min)          *         100ULL +
        static_cast<uint64_t>(lt.tm_sec);

    const std::string relPath =
        (fs::path("src") / "objects" / objPath.filename()).generic_string();

    std::ofstream meta(fs::path(objPath.string() + ".meta"), std::ios::trunc);
    if (!meta) { LogError("Failed to create meta for: " + objPath.string()); return; }

    meta << "ID="   << id      << '\n'
         << "Path=" << relPath << '\n';

    if (!meta.good()) LogError("Failed to write meta for: " + objPath.string());
}


// Parsing .obj file

bool AssetSystem::ParseOBJ(const fs::path& objPath, MeshData& out) {
    std::ifstream file(objPath);
    if (!file) { LogError("Cannot open OBJ: " + objPath.string()); return false; }

    std::vector<float> positions;
    std::vector<float> texcoords;

    struct Corner {
        int v = 0;
        int vt = 0;
    };

    auto emitCorner = [&](const Corner& corner) -> bool {
        int vertexIndex = corner.v;
        int texIndex = corner.vt;

        const int positionCount = static_cast<int>(positions.size() / 3);
        const int texCount = static_cast<int>(texcoords.size() / 2);

        if (vertexIndex < 0) {
            vertexIndex = positionCount + vertexIndex + 1;
        }
        if (texIndex < 0) {
            texIndex = texCount + texIndex + 1;
        }

        const int vi = vertexIndex - 1;
        if (vi < 0 || vi * 3 + 2 >= static_cast<int>(positions.size())) {
            LogError("Face vertex out of range in OBJ: " + objPath.string());
            return false;
        }

        const float x = positions[vi * 3 + 0];
        const float y = positions[vi * 3 + 1];
        const float z = positions[vi * 3 + 2];
        out.vertices.insert(out.vertices.end(), { x, y, z });

        if (texIndex > 0 && (texIndex - 1) * 2 + 1 < static_cast<int>(texcoords.size())) {
            out.textures.push_back(texcoords[(texIndex - 1) * 2 + 0]);
            out.textures.push_back(1.0f - texcoords[(texIndex - 1) * 2 + 1]); // Invert V: OBJ V=0 is bottom, texture V=0 is top
        }
        else {
            out.textures.push_back(x * 0.5f + 0.5f);
            out.textures.push_back(y * 0.5f + 0.5f);
        }

        return true;
    };

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        std::istringstream ss(line);
        std::string token;
        ss >> token;

        if (token == "v") {
            float x, y, z;
            if (!(ss >> x >> y >> z)) { LogError("Malformed vertex in OBJ: " + objPath.string()); return false; }
            positions.insert(positions.end(), { x, y, z });
        }
        else if (token == "vt") {
            float u, v;
            if (!(ss >> u >> v)) { LogError("Malformed texcoord in OBJ: " + objPath.string()); return false; }
            texcoords.insert(texcoords.end(), { u, v });
        }
        else if (token == "f") {
            std::vector<Corner> face;
            std::string ft;
            while (ss >> ft) {
                Corner corner;
                try {
                    const auto firstSlash = ft.find('/');
                    if (firstSlash == std::string::npos) {
                        corner.v = std::stoi(ft);
                    }
                    else {
                        corner.v = std::stoi(ft.substr(0, firstSlash));
                        const auto secondSlash = ft.find('/', firstSlash + 1);
                        const std::string vtPart = (secondSlash == std::string::npos)
                            ? ft.substr(firstSlash + 1)
                            : ft.substr(firstSlash + 1, secondSlash - firstSlash - 1);
                        if (!vtPart.empty()) {
                            corner.vt = std::stoi(vtPart);
                        }
                    }
                }
                catch (...) {
                    LogError("Invalid face index in OBJ: " + objPath.string());
                    return false;
                }
                face.push_back(corner);
            }

            if (face.size() < 3) {
                continue;
            }

            for (size_t i = 1; i + 1 < face.size(); ++i) {
                const unsigned int base = static_cast<unsigned int>(out.vertices.size() / 3);
                if (!emitCorner(face[0]) || !emitCorner(face[i]) || !emitCorner(face[i + 1])) {
                    return false;
                }
                out.indices.push_back(base);
                out.indices.push_back(base + 1);
                out.indices.push_back(base + 2);
            }
        }
    }

    return !out.vertices.empty() && !out.indices.empty();
}

// Writing .bin file 

bool AssetSystem::WriteBinOBJ(const fs::path& binPath, const MeshData& mesh) {
    if (mesh.vertices.size() > UINT32_MAX || mesh.indices.size() > UINT32_MAX || mesh.textures.size() > UINT32_MAX) {
        LogError("Mesh too large for .bin: " + binPath.string());
        return false;
    }

    try { fs::create_directories(binPath.parent_path()); }
    catch (const fs::filesystem_error& e) {
        LogError("Cannot create bins directory: " + std::string(e.what()));
        return false;
    }

    std::ofstream bin(binPath, std::ios::binary | std::ios::trunc);
    if (!bin) { LogError("Cannot open .bin for writing: " + binPath.string()); return false; }

    const uint32_t vc = static_cast<uint32_t>(mesh.vertices.size());
    const uint32_t ic = static_cast<uint32_t>(mesh.indices.size());
    const uint32_t tc = static_cast<uint32_t>(mesh.textures.size());

    bin.write(reinterpret_cast<const char*>(&vc), sizeof(vc));
    bin.write(reinterpret_cast<const char*>(&ic), sizeof(ic));
    bin.write(reinterpret_cast<const char*>(&tc), sizeof(tc));
    if (!mesh.vertices.empty()) bin.write(reinterpret_cast<const char*>(mesh.vertices.data()), vc * sizeof(float));
    if (!mesh.indices.empty())  bin.write(reinterpret_cast<const char*>(mesh.indices.data()),  ic * sizeof(unsigned int));
    if (!mesh.textures.empty()) bin.write(reinterpret_cast<const char*>(mesh.textures.data()), tc * sizeof(float));

    if (!bin.good()) { LogError("Write error for .bin: " + binPath.string()); return false; }
    return true;
}

