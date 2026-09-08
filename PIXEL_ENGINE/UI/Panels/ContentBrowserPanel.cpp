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

}

void ContentBrowserPanel::setEventQueue(std::vector<EditorEvent>* events) {
    m_events = events;
}

void ContentBrowserPanel::setConsole(ConsolePanel* console) {
    m_console = console;
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
    if (ImGui::Button("Register Object")) {
        registerObject();
    }

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputTextWithHint("##Search", "Search...", m_searchBuffer, sizeof(m_searchBuffer));
}

void ContentBrowserPanel::drawFolderTree() {
    ImGui::BeginChild("FolderTree", ImVec2(0, 0), false);

    std::filesystem::create_directories("src/objects");

    if (ImGui::TreeNodeEx("src", ImGuiTreeNodeFlags_DefaultOpen)) {
        const std::string objectsPath = "src/objects";
        if (ImGui::Selectable("objects", m_currentDirectory == objectsPath)) {
            m_currentDirectory = objectsPath;
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
            Logger::getInstance().addLog(Logger::LogEntry::LOG_INFO,
                "Object registration queued for " + event.name + ".");
        }
    }
#else
    Logger::getInstance().addLog(Logger::LogEntry::LOG_WARNING,
        "Object registration is supported only on Windows.");
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

            // ??? ??? ?????????? ????????
            if (isSelected) {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.3f, 0.5f, 0.8f, 0.3f));
            }

            ImGui::BeginChild(("##item" + std::to_string(i)).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, rowHeight), true);

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);

            // ?????? (????????? ??????)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.9f, 1.0f, 1.0f));
            ImGui::Text("[OBJ]");
            ImGui::PopStyleColor();
            ImGui::SameLine(0, 12.0f);

            // ???????? ?????
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

    unsigned long long int meshID = 0;
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
        Logger::getInstance().addLog(Logger::LogEntry::LOG_ERROR,
            "Invalid mesh ID in meta file: " + path);
        return;
    }

    EditorEvent event;
    event.type = EditorEventType::AddObject;
    event.meshID = meshID;
    event.name = stem;
    pushEvent(event);

    Logger::getInstance().addLog(Logger::LogEntry::LOG_INFO, "Add object event queued for " + filename + ".");
}

void ContentBrowserPanel::pushEvent(const EditorEvent& event) {
    if (m_events) {
        // /FLAG Stores the event flag for the engine-side event processor.
        m_events->push_back(event);
    }
}
