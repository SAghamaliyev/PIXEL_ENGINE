#include "ContentBrowserPanel.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "Comdlg32.lib")
#endif

#include <imgui.h>

#include "Core/EventSystem/EventSystem.h"
#include "Core/Logger/Logger.h"
#include "Systems/UI/Theme/EditorTheme.h"

namespace {

bool isObjExtension(const std::filesystem::path& path) {
    return path.extension() == ".obj";
}

bool isTextureExtension(const std::filesystem::path& path) {
    const std::filesystem::path ext = path.extension();
    return ext == ".png" || ext == ".jpg" || ext == ".jpeg";
}

}

void ContentBrowserPanel::setTextureSelectionMode(bool enabled, long entityID) {
    m_textureSelectionMode = enabled;
    m_selectedEntityIDForTexture = entityID;
    if (enabled) {
        m_currentDirectory = "src/Textures";
    }
    else {
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
    ImGui::Begin("##ContentBrowser", nullptr, EDITOR_PANEL_WINDOW_FLAGS);
    EditorTheme::drawPanelHeader(
        "Content Browser",
        m_textureSelectionMode ? "Select a texture" : "Project files");

    drawToolbar();
    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    ImGui::Columns(2, "ContentBrowserColumns", true);
    ImGui::SetColumnWidth(0, 140.0f);

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
    }
    else {
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            m_textureSelectionMode = false;
            m_currentDirectory = "src/objects";
            m_selectedFilePath.clear();
        }
    }

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputTextWithHint("##Search", "Search assets...", m_searchBuffer, sizeof(m_searchBuffer));
}

void ContentBrowserPanel::drawFolderTree() {
    ImGui::BeginChild("FolderTree", ImVec2(0.0f, 0.0f), true);

    std::filesystem::create_directories("src/objects");
    std::filesystem::create_directories("src/Textures");

    if (ImGui::TreeNodeEx("src", ImGuiTreeNodeFlags_DefaultOpen)) {
        const std::string objects_path = "src/objects";
        if (ImGui::Selectable("objects", m_currentDirectory == objects_path)) {
            m_currentDirectory = objects_path;
            m_selectedFilePath.clear();
        }

        const std::string textures_path = "src/Textures";
        if (ImGui::Selectable("Textures", m_currentDirectory == textures_path)) {
            m_currentDirectory = textures_path;
            m_selectedFilePath.clear();
        }

        ImGui::TreePop();
    }

    ImGui::EndChild();
}

void ContentBrowserPanel::registerObject() {
#ifdef _WIN32
    char file_name[MAX_PATH] = {};
    OPENFILENAMEA dialog = {};
    dialog.lStructSize = sizeof(dialog);
    dialog.lpstrFile = file_name;
    dialog.nMaxFile = sizeof(file_name);
    dialog.lpstrFilter = "OBJ files (*.obj)\0*.obj\0\0";
    dialog.nFilterIndex = 1;
    dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&dialog) != 0) {
        const std::filesystem::path selected_path(file_name);
        if (isObjExtension(selected_path)) {
            EditorEvent register_event;
            register_event.type = EditorEventType::RegisterMesh;
            register_event.info.path = selected_path.string();
            register_event.info.name = selected_path.filename().string();
            EventSystem::pushEvent(register_event);
            Logger::addLog(
                LogType::LOG_INFO,
                "Object registration queued for " + register_event.info.name + ".");
        }
    }
#else
    Logger::addLog(
        LogType::LOG_WARNING,
        "Object registration is supported only on Windows.");
#endif
}

void ContentBrowserPanel::registerTexture() {
#ifdef _WIN32
    char file_name[MAX_PATH] = {};
    OPENFILENAMEA dialog = {};
    dialog.lStructSize = sizeof(dialog);
    dialog.lpstrFile = file_name;
    dialog.nMaxFile = sizeof(file_name);
    dialog.lpstrFilter = "Image files (*.png; *.jpg; *.jpeg)\0*.png;*.jpg;*.jpeg\0PNG files (*.png)\0*.png\0JPG files (*.jpg; *.jpeg)\0*.jpg;*.jpeg\0All files (*.*)\0*.*\0\0";
    dialog.nFilterIndex = 1;
    dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&dialog) != 0) {
        const std::filesystem::path selected_path(file_name);
        if (isTextureExtension(selected_path)) {
            EditorEvent register_event;
            register_event.type = EditorEventType::RegisterTexture;
            register_event.info.path = selected_path.string();
            register_event.info.name = selected_path.filename().string();
            EventSystem::pushEvent(register_event);
            Logger::addLog(
                LogType::LOG_INFO,
                "Texture registration queued for " + register_event.info.name + ".");
        }
    }
#else
    Logger::addLog(
        LogType::LOG_WARNING,
        "Texture registration is supported only on Windows.");
#endif
}

void ContentBrowserPanel::drawFileView() {
    ImGui::BeginChild("FileView", ImVec2(0.0f, 0.0f), true);

    constexpr float ICON_SIZE = 28.0f;
    const float row_height = ICON_SIZE + 6.0f;
    int item_index = 0;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(m_currentDirectory)) {
            if (entry.is_directory()) {
                continue;
            }

            if (!m_textureSelectionMode) {
                if (entry.path().extension() != ".meta") {
                    continue;
                }

                const std::string meta_filename = entry.path().filename().string();
                const std::string filename = entry.path().stem().string();
                if (m_searchBuffer[0] != '\0' && filename.find(m_searchBuffer) == std::string::npos) {
                    continue;
                }

                const std::string file_path = entry.path().string();
                const bool is_selected = (m_selectedFilePath == file_path);

                ImGui::PushID(item_index++);

                if (is_selected) {
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.24f, 0.42f, 0.62f, 0.35f));
                }

                ImGui::BeginChild(("##item" + std::to_string(item_index)).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, row_height), true);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.0f);

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.70f, 0.86f, 1.0f, 1.0f));
                ImGui::Text("[OBJ]");
                ImGui::PopStyleColor();
                ImGui::SameLine(0.0f, 10.0f);

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
                ImGui::Text("%s", filename.c_str());

                ImGui::EndChild();

                if (ImGui::IsItemClicked()) {
                    m_selectedFilePath = file_path;
                }

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                    queueAddObjectEvent(file_path, meta_filename, filename);
                }

                if (ImGui::BeginPopupContextItem()) {
                    if (ImGui::MenuItem("Add to Scene")) {
                        queueAddObjectEvent(file_path, meta_filename, filename);
                    }
                    ImGui::EndPopup();
                }

                if (is_selected) {
                    ImGui::PopStyleColor();
                }

                ImGui::PopID();
            }
            else {
                if (entry.path().extension() != ".meta") {
                    continue;
                }

                const std::string meta_path = entry.path().string();
                const std::string texture_path = meta_path.substr(0, meta_path.length() - 5);
                if (!isTextureExtension(std::filesystem::path(texture_path))) {
                    continue;
                }

                const std::string filename = entry.path().stem().string();
                if (m_searchBuffer[0] != '\0' && filename.find(m_searchBuffer) == std::string::npos) {
                    continue;
                }

                const std::string file_path = entry.path().string();
                const bool is_selected = (m_selectedFilePath == file_path);

                ImGui::PushID(item_index++);

                if (is_selected) {
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.24f, 0.42f, 0.62f, 0.35f));
                }

                ImGui::BeginChild(("##item" + std::to_string(item_index)).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, row_height), true);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.0f);

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.82f, 0.38f, 1.0f));
                ImGui::Text("[TEX]");
                ImGui::PopStyleColor();
                ImGui::SameLine(0.0f, 10.0f);

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
                ImGui::Text("%s", filename.c_str());

                ImGui::EndChild();

                if (ImGui::IsItemClicked()) {
                    m_selectedFilePath = file_path;
                }

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                    queueAssignTextureEvent(file_path, entry.path().filename().string(), filename);
                }

                if (is_selected) {
                    ImGui::PopStyleColor();
                }

                ImGui::PopID();
            }
        }
    }
    catch (...) {
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Error reading directory");
    }

    ImGui::EndChild();
}

void ContentBrowserPanel::queueAddObjectEvent(const std::string& metaPath, const std::string& filename, const std::string& stem) {
    std::ifstream meta_file(metaPath);
    if (!meta_file.is_open()) {
        return;
    }

    uint64_t mesh_id = 0;
    std::string line;
    while (std::getline(meta_file, line)) {
        if (line.rfind("ID=", 0) == 0) {
            try {
                mesh_id = std::stoull(line.substr(3));
            }
            catch (...) {
                mesh_id = 0;
            }
            break;
        }
    }

    if (mesh_id == 0) {
        Logger::addLog(
            LogType::LOG_ERROR,
            "Invalid mesh ID in meta file: " + metaPath);
        return;
    }

    EditorEvent add_event;
    add_event.type = EditorEventType::AddEntity;
    add_event.info.meshID = mesh_id;
    add_event.info.name = stem;
    EventSystem::pushEvent(add_event);

    Logger::addLog(LogType::LOG_INFO, "Add object event queued for " + filename + ".");
}

void ContentBrowserPanel::queueAssignTextureEvent(const std::string& metaPath, const std::string& filename, const std::string& stem) {
    std::ifstream meta_file(metaPath);
    if (!meta_file.is_open()) {
        Logger::addLog(LogType::LOG_ERROR, "Cannot open texture meta file: " + metaPath);
        return;
    }

    uint64_t texture_id = 0;
    std::string line;
    while (std::getline(meta_file, line)) {
        if (line.rfind("ID=", 0) == 0) {
            try {
                texture_id = std::stoull(line.substr(3));
            }
            catch (...) {
                texture_id = 0;
            }
            break;
        }
    }

    if (texture_id == 0) {
        Logger::addLog(
            LogType::LOG_ERROR,
            "Invalid texture ID in meta file: " + metaPath);
        return;
    }

    EditorEvent assign_event;
    assign_event.type = EditorEventType::AssignTexture;
    assign_event.info.entityID = m_selectedEntityIDForTexture;
    assign_event.info.textureID = texture_id;
    assign_event.info.name = stem;
    EventSystem::pushEvent(assign_event);

    m_textureSelectionMode = false;
    m_currentDirectory = "src/objects";
    m_selectedFilePath.clear();
    memset(m_searchBuffer, 0, sizeof(m_searchBuffer));

    Logger::addLog(
        LogType::LOG_INFO,
        "Assign texture event queued. Entity ID: " + std::to_string(m_selectedEntityIDForTexture) +
        ", Texture ID: " + std::to_string(texture_id) +
        ", File: " + filename + ".");
}
