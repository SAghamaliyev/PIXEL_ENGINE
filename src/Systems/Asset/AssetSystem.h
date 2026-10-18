#pragma once

#include <filesystem>
#include <string>

class AssetSystem {
public:
    // Scans project assets directory and compiles missing binaries
    void run();

    // Registers a file into the project, creates metadata and compiles binary cache
    void registerFile(const std::string& path);

private:
    // Scans a specific directory (Meshes and Textures) and processes all assets inside it
    void processDirectory(const std::filesystem::path& directory);

    // Reads ID from .meta file. If meta doesn't exist, calls createMeta() and reads again.
    // Logs an error and returns 0 if meta creation or reading fails.
    uint64_t getMeta(const std::filesystem::path& assetPath);

    // Generates a unique 64-bit ID and writes a new .meta file next to the asset
    void createMeta(const std::filesystem::path& assetPath);

    // Checks if <id>.bin exists and is fresh. If missing or outdated, triggers compilation
    void prepareToCompileAsset(const std::filesystem::path& assetPath, uint64_t id);

    // Parses OBJ mesh and writes compiled binary cache directly into binPath
    void compileMesh(const std::filesystem::path& srcPath, const std::filesystem::path& binPath);

    // Loads texture image (PNG, JPG) and writes raw pixel binary cache directly into binPath
    void compileTexture(const std::filesystem::path& srcPath, const std::filesystem::path& binPath);
};