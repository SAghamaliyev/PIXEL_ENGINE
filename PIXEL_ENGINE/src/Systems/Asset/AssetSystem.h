#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

// Data structures which will be written into .bin files:
struct MeshAssetData {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> textures;
};

struct TextureAssetData {
    std::vector<unsigned char> data; // Raw pixel buffer (RGBA bytes)
    int width = 0;                   // Image width in pixels
    int height = 0;                  // Image height in pixels
    int nrChannels = 0;              // Number of color channels (3 for RGB, 4 for RGBA)
};

// Metadata about an asset file
// This information is stored in .meta files and links original assets to their binary representations
struct AssetInfo {
    uint64_t id = 0;             // ID of its .bin representation
    std::filesystem::path path;  // Path to the original asset(.obj, .png, .jpeg and etc.)
};

class AssetSystem {
public:
    // Processes all registered asset files and creates/updates their binary representations
    void run();

    // Registers a new asset file and prepares it for processing
    void registerFile(const std::string& path);

private:
    // Loads metadata from a .meta file and populates AssetInfo structure
    bool loadMeta(const std::filesystem::path& metaPath, AssetInfo& outInfo);

    // Texture file operations (.png, .jpg, .jpeg)

        // Scans Textures directory for texture files and generates binary cache if missing
        void runTexture();

        // Registers a texture file by copying it to Textures folder and creating metadata
        void registerTextureFile(const std::string& path);

        // Generates .meta file with unique ID and path for a texture asset
        void makeTextureMeta(const std::filesystem::path& texturePath);

        // Reads a texture file and take data from it
         bool parseTexture(const std::filesystem::path& texturePath, 
                           TextureAssetData& outTexture);

        // Writes parsed texture data to binary cache file for fast loading
        bool writeTextureBin(const std::filesystem::path& binPath,
                             const TextureAssetData& texture);

    // Mesh file operations (for a while only for .obj)

        // Scans Objects directory for mesh files and generates binary cache if missing
        void runMesh();

        // Registers a mesh file by copying it to Objects folder and creating metadata
        void registerMeshFile(const std::string& path);

        // Generates .meta file with ID and path for a mesh asset
        void makeMeshMeta(const std::filesystem::path& meshPath);

        // Parses a .obj file and extracts vertices, indices, and texture coordinates
        bool parseMesh(const std::filesystem::path& meshPath, 
                       MeshAssetData& outMesh);

        // Writes parsed mesh data to binary cache file for fast loading
        bool writeMeshBin(const std::filesystem::path& binPath,
                          const MeshAssetData& mesh);
};
