#include "ContentBrowserPanel.h"
#include "ConsolePanel.h"
#include "../../Logger/Logger.h"

#include "../Materials/imgui.h"

#include <algorithm>
#include <filesystem>

namespace {

bool isModelExtension(const std::filesystem::path& path) {
    const std::filesystem::path ext = path.extension();
    return ext == ".obj" || ext == ".fbx";
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
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
    ImGui::InputTextWithHint("##Search", "Search...", m_searchBuffer, sizeof(m_searchBuffer));

    ImGui::SameLine();
    if (ImGui::Button("Root")) {
        m_currentDirectory = "src";
        m_selectedFilePath.clear();
    }

    ImGui::SameLine();
    if (ImGui::Button("Up")) {
        const std::filesystem::path current(m_currentDirectory);
        if (current.has_parent_path()) {
            m_currentDirectory = current.parent_path().string();
            m_selectedFilePath.clear();
        }
    }
}

void ContentBrowserPanel::drawFolderTree() {
    ImGui::BeginChild("FolderTree", ImVec2(0, 0), false);

    if (ImGui::TreeNodeEx("src", ImGuiTreeNodeFlags_DefaultOpen)) {
        try {
            for (const auto& entry : std::filesystem::directory_iterator("src")) {
                if (!entry.is_directory()) {
                    continue;
                }

                const std::string folderPath = entry.path().string();
                const std::string folderName = entry.path().filename().string();
                if (ImGui::Selectable(folderName.c_str(), m_currentDirectory == folderPath)) {
                    m_currentDirectory = folderPath;
                    m_selectedFilePath.clear();
                }
            }
        } catch (...) {
        }
        ImGui::TreePop();
    }

    ImGui::EndChild();
}

void ContentBrowserPanel::drawFileView() {
    ImGui::BeginChild("FileView", ImVec2(0, 0), false);

    ImGui::TextDisabled("Path: %s", m_currentDirectory.c_str());
    ImGui::Separator();

    const float iconSize = 32.0f;
    const float rowHeight = iconSize + 8.0f;
    int i = 0;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(m_currentDirectory)) {
            if (entry.is_directory()) {
                continue;
            }

            // ?????????? ?????? .obj ?????
            if (!isModelExtension(entry.path())) {
                continue;
            }

            const std::string filename = entry.path().filename().string();
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
                queueAddObjectEvent(filePath, filename, entry.path().stem().string());
            }

            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Add to Scene")) {
                    queueAddObjectEvent(filePath, filename, entry.path().stem().string());
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
    const std::filesystem::path filePath(path);
    if (!isModelExtension(filePath)) {
        return;
    }

    std::string pathString = path;
    std::replace(pathString.begin(), pathString.end(), '\\', '/');

    // /FLAG AddObject: UI requests adding a model asset by path and display name.
    EditorEvent event;
    event.type = EditorEventType::AddObject;
    event.path = pathString;
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
