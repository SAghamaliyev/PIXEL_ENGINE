#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "../Core/Definitions/SceneDefinitons.h"
#include "../Core/Logger/Logger.h"

struct MeshData {
    std::vector<float>        vertices;
    std::vector<unsigned int> indices;
    std::vector<float>        textures;
};

struct TexData {
    std::vector<unsigned char> data;
    int width      = 0;
    int height     = 0;
    int nrChannels = 0;
};

struct ShaderData {
    MaterialType Type;
    std::filesystem::path vertexPath;
    std::filesystem::path fragmentPath;
};

struct AssetInfo {
    uint64_t id = 0;
    std::filesystem::path path;
};

class AssetSystem {
public:
    void Run();
    void RegisterFile(const std::string& path);

private:

    bool LoadMeta(const std::filesystem::path& metaPath, AssetInfo& out);

    // .png/.jpg (TEXTURES) file functions
    void RunTEX();
    void RegisterFileTEX(const std::string& path);
    void MakeFileMetaTEX(const std::filesystem::path& texPath);
    bool ParseTEX(const std::filesystem::path& texPath, TexData& out);
    bool WriteBinTEX(const std::filesystem::path& binPath, const TexData& tex);

    // .obj file functions
    void RunOBJ();
    void RegisterFileOBJ(const std::string& path);
    void MakeFileMetaOBJ(const std::filesystem::path& objPath);
    bool ParseOBJ(const std::filesystem::path& objPath, MeshData& out);
    bool WriteBinOBJ(const std::filesystem::path& binPath, const MeshData& mesh);

    static void LogError(const std::string& msg);
};
