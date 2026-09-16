#include "ContentBrowserPanel.h"
#include "ConsolePanel.h"
#include "../../Logger/Logger.h"

#include "../Materials/imgui.h"

#include <algorithm>
#include <filesystem>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "Comdlg32.lib")
#endif

namespace {

bool isModelExtension(const std::filesystem::path& path) {
    const std::filesystem::path ext = path.extension();
    return ext == ".obj" || ext == ".fbx";
}

bool isObjExtension(const std::filesystem::path& path) {
    return path.extension() == ".obj";
}

bool isTextureExtension(const std::filesystem::path& path) {
    const std::filesystem::path ext = path.extension();
    return ext == ".png" || ext == ".jpg" || ext == ".jpeg";
}

}

void ContentBrowserPanel::setEventQueue(EventSystem* events) {
    m_events = events;
}

void ContentBrowserPanel::setConsole(ConsolePanel* console) {
    m_console = console;
}

void ContentBrowserPanel::setTextureSelectionMode(bool enabled, unsigned int entityID) {
    m_textureSelectionMode = enabled;
    m_selectedEntityIDForTexture = entityID;
    if (enabled) {
        m_currentDirectory = "src/Textures";
    } else {
        m_currentDirectory = "src/objects";
    }
    m_selectedFilePath.clear();
    memset(m_searchBuffer, 0, sizeof(m_searchBuffer));
}

void ContentBrowserPanel::draw(const EditorLayout& layout) {
    if (!m_visible) {
        return;
    }

    applyPanelRect(layout.contentBrowser);
    ImGui::Begin("Content Browser", nullptr, kEditorPanelWindowFlags);

    drawToolbar();
    ImGui::Separator();

    ImGui::Columns(2, "ContentBrowserColumns", true);
    ImGui::SetColumnWidth(0, 150.0f);

    drawFolderTree();

    ImGui::NextColumn();
    drawFileView();

    ImGui::Columns(1);
    ImGui::End();
}

void ContentBrowserPanel::drawToolbar() {
    if (ImGui::Button("Home")) {
        m_currentDirectory = "src";
        m_selectedFilePath.clear();
    }

    ImGui::SameLine();
    if (ImGui::Button("Objects")) {
        std::filesystem::create_directories("src/objects");
        m_currentDirectory = "src/objects";
        m_selectedFilePath.clear();
    }

    ImGui::SameLine();
    if (ImGui::Button("Textures")) {
        std::filesystem::create_directories("src/Textures");
        m_currentDirectory = "src/Textures";
        m_selectedFilePath.clear();
    }

    if (!m_textureSelectionMode) {
        ImGui::SameLine();
        if (ImGui::Button("Register Object")) {
            registerObject();
        }
        ImGui::SameLine();
        if (ImGui::Button("Register Texture")) {
            registerTexture();
        }
    } else {
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            m_textureSelectionMode = false;
            m_currentDirectory = "src/objects";
            m_selectedFilePath.clear();
        }
    }

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputTextWithHint("##Search", "Search...", m_searchBuffer, sizeof(m_searchBuffer));
}

void ContentBrowserPanel::drawFolderTree() {
    ImGui::BeginChild("FolderTree", ImVec2(0, 0), false);

    std::filesystem::create_directories("src/objects");
    std::filesystem::create_directories("src/Textures");

    if (ImGui::TreeNodeEx("src", ImGuiTreeNodeFlags_DefaultOpen)) {
        const std::string objectsPath = "src/objects";
        if (ImGui::Selectable("objects", m_currentDirectory == objectsPath)) {
            m_currentDirectory = objectsPath;
            m_selectedFilePath.clear();
        }

        const std::string texturesPath = "src/Textures";
        if (ImGui::Selectable("Textures", m_currentDirectory == texturesPath)) {
            m_currentDirectory = texturesPath;
            m_selectedFilePath.clear();
        }

        ImGui::TreePop();
    }

    ImGui::EndChild();
}

void ContentBrowserPanel::registerObject() {
#ifdef _WIN32
    char fileName[MAX_PATH] = {};
    OPENFILENAMEA dialog = {};
    dialog.lStructSize = sizeof(dialog);
    dialog.lpstrFile = fileName;
    dialog.nMaxFile = sizeof(fileName);
    dialog.lpstrFilter = "OBJ files (*.obj)\0*.obj\0\0";
    dialog.nFilterIndex = 1;
    dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&dialog) != 0) {
        const std::filesystem::path selectedPath(fileName);
        if (isObjExtension(selectedPath)) {
            EditorEvent event;
            event.type = EditorEventType::RegisterObject;
            event.path = selectedPath.string();
            event.name = selectedPath.filename().string();
            pushEvent(event);
            Logger::addLog(LOG_INFO,
                "Object registration queued for " + event.name + ".");
        }
    }
#else
    Logger::addLog(LOG_WARNING,
        "Object registration is supported only on Windows.");
#endif
}

void ContentBrowserPanel::registerTexture() {
#ifdef _WIN32
    char fileName[MAX_PATH] = {};
    OPENFILENAMEA dialog = {};
    dialog.lStructSize = sizeof(dialog);
    dialog.lpstrFile = fileName;
    dialog.nMaxFile = sizeof(fileName);
    dialog.lpstrFilter = "Image files (*.png; *.jpg; *.jpeg)\0*.png;*.jpg;*.jpeg\0PNG files (*.png)\0*.png\0JPG files (*.jpg; *.jpeg)\0*.jpg;*.jpeg\0All files (*.*)\0*.*\0\0";
    dialog.nFilterIndex = 1;
    dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&dialog) != 0) {
        const std::filesystem::path selectedPath(fileName);
        if (isTextureExtension(selectedPath)) {
            EditorEvent event;
            event.type = EditorEventType::RegisterTexture;
            event.path = selectedPath.string();
            event.name = selectedPath.filename().string();
            pushEvent(event);
            Logger::addLog(LOG_INFO,
                "Texture registration queued for " + event.name + ".");
        }
    }
#else
    Logger::addLog(LOG_WARNING,
        "Texture registration is supported only on Windows.");
#endif
}

void ContentBrowserPanel::drawFileView() {
    ImGui::BeginChild("FileView", ImVec2(0, 0), false);

    const float iconSize = 32.0f;
    const float rowHeight = iconSize + 8.0f;
    int i = 0;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(m_currentDirectory)) {
            if (entry.is_directory()) {
                continue;
            }

            if (!m_textureSelectionMode) {
                // Normal mode - show objects (OBJ files with .meta)
                if (entry.path().extension() != ".meta") {
                    continue;
                }

                const std::string metaFilename = entry.path().filename().string();
                const std::string filename = entry.path().stem().string();
                if (m_searchBuffer[0] != '\0' && filename.find(m_searchBuffer) == std::string::npos) {
                    continue;
                }

                const std::string filePath = entry.path().string();
                const bool isSelected = (m_selectedFilePath == filePath);

                ImGui::PushID(i++);

                if (isSelected) {
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.3f, 0.5f, 0.8f, 0.3f));
                }

                ImGui::BeginChild(("##item" + std::to_string(i)).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, rowHeight), true);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.9f, 1.0f, 1.0f));
                ImGui::Text("[OBJ]");
                ImGui::PopStyleColor();
                ImGui::SameLine(0, 12.0f);

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.0f);
                ImGui::Text("%s", filename.c_str());

                ImGui::EndChild();

                if (ImGui::IsItemClicked()) {
                    m_selectedFilePath = filePath;
                }

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                    queueAddObjectEvent(filePath, metaFilename, filename);
                }

                if (ImGui::BeginPopupContextItem()) {
                    if (ImGui::MenuItem("Add to Scene")) {
                        queueAddObjectEvent(filePath, metaFilename, filename);
                    }
                    ImGui::EndPopup();
                }

                if (isSelected) {
                    ImGui::PopStyleColor();
                }

                ImGui::PopID();
            } else {
                // Texture selection mode - show textures (image files with .meta)
                if (entry.path().extension() != ".meta") {
                    continue;
                }

                // Check if the corresponding image file exists
                const std::string metaPath = entry.path().string();
                const std::string texturePath = metaPath.substr(0, metaPath.length() - 5); // Remove ".meta"
                if (!isTextureExtension(std::filesystem::path(texturePath))) {
                    continue;
                }

                const std::string filename = entry.path().stem().string();
                if (m_searchBuffer[0] != '\0' && filename.find(m_searchBuffer) == std::string::npos) {
                    continue;
                }

                const std::string filePath = entry.path().string();
                const bool isSelected = (m_selectedFilePath == filePath);

                ImGui::PushID(i++);

                if (isSelected) {
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.3f, 0.5f, 0.8f, 0.3f));
                }

                ImGui::BeginChild(("##item" + std::to_string(i)).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, rowHeight), true);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.3f, 1.0f));
                ImGui::Text("[TEX]");
                ImGui::PopStyleColor();
                ImGui::SameLine(0, 12.0f);

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6.0f);
                ImGui::Text("%s", filename.c_str());

                ImGui::EndChild();

                if (ImGui::IsItemClicked()) {
                    m_selectedFilePath = filePath;
                }

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                    queueAssignTextureEvent(filePath, entry.path().filename().string(), filename);
                }

                if (isSelected) {
                    ImGui::PopStyleColor();
                }

                ImGui::PopID();
            }
        }
    } catch (...) {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Error reading directory");
    }

    ImGui::EndChild();
}

void ContentBrowserPanel::queueAddObjectEvent(const std::string& path, const std::string& filename, const std::string& stem) {
    std::ifstream metaFile(path);
    if (!metaFile.is_open()) {
        return;
    }

    uint64_t meshID = 0;
    std::string line;
    while (std::getline(metaFile, line)) {
        if (line.rfind("ID=", 0) == 0) {
            try {
                meshID = std::stoull(line.substr(3));
            }
            catch (...) {
                meshID = 0;
            }
            break;
        }
    }

    if (meshID == 0) {
        Logger::addLog(LOG_ERROR,
            "Invalid mesh ID in meta file: " + path);
        return;
    }

    EditorEvent event;
    event.type = EditorEventType::AddObject;
    event.meshID = meshID;
    event.name = stem;
    pushEvent(event);

    Logger::addLog(LOG_INFO, "Add object event queued for " + filename + ".");
}

void ContentBrowserPanel::queueAssignTextureEvent(const std::string& metaPath, const std::string& filename, const std::string& stem) {
    std::ifstream metaFile(metaPath);
    if (!metaFile.is_open()) {
        Logger::addLog(LOG_ERROR, "Cannot open texture meta file: " + metaPath);
        return;
    }

    uint64_t textureID = 0;
    std::string line;
    while (std::getline(metaFile, line)) {
        if (line.rfind("ID=", 0) == 0) {
            try {
                textureID = std::stoull(line.substr(3));
            }
            catch (...) {
                textureID = 0;
            }
            break;
        }
    }

    if (textureID == 0) {
        Logger::addLog(LOG_ERROR,
            "Invalid texture ID in meta file: " + metaPath);
        return;
    }

    EditorEvent event;
    event.type = EditorEventType::AssignTexture;
    event.entityID = m_selectedEntityIDForTexture;
    event.textureID = textureID;
    event.name = stem;
    pushEvent(event);

    m_textureSelectionMode = false;
    m_currentDirectory = "src/objects";
    m_selectedFilePath.clear();
    memset(m_searchBuffer, 0, sizeof(m_searchBuffer));

    Logger::addLog(LOG_INFO, 
        "Assign texture event queued. Entity ID: " + std::to_string(m_selectedEntityIDForTexture) + 
        ", Texture ID: " + std::to_string(textureID) + 
        ", File: " + filename + ".");
}

void ContentBrowserPanel::pushEvent(const EditorEvent& event) {
    if (m_events) {
        // /FLAG Stores the event flag for the engine-side event processor.
        m_events->push(event);
    }
}
