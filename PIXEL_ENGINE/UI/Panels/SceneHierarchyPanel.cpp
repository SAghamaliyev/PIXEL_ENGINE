#include "SceneHierarchyPanel.h"

#include "../Theme/EditorTheme.h"
#include "../Materials/imgui.h"
#include "../../Core/EventSystem/EventSystem.h"
#include "../../Core/Logger/Logger.h"

#include <cstring>

void SceneHierarchyPanel::setHierarchyViews(const std::vector<EditorHierarchyView>* hierarchyViews) {
    m_hierarchyViews = hierarchyViews;
}

void SceneHierarchyPanel::draw(const EditorLayout& layout) {
    if (!m_visible) {
        return;
    }

    applyPanelRect(layout.hierarchy);
    ImGui::Begin("##SceneHierarchy", nullptr, kEditorPanelWindowFlags);
    EditorTheme::drawPanelHeader("Hierarchy", "Scene");

    if (ImGui::Button("Add Entity", ImVec2(ImGui::GetContentRegionAvail().x * 0.58f, 0.0f))) {
        EditorEvent event;
        event.type = EditorEventType::AddObject;
        event.info.isEmptyEntity = true;
        EventSystem::pushEvent(event);
        Logger::addLog(LOG_INFO, "Add Entity event queued.");
    }

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.45f, 0.18f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.62f, 0.24f, 0.26f, 1.0f));
    if (ImGui::Button("Delete", ImVec2(-1.0f, 0.0f))) {
        if (m_selectedEntityID >= 0) {
            EditorEvent event;
            event.type = EditorEventType::DeleteObject;
            event.info.entityID = m_selectedEntityID;
            EventSystem::pushEvent(event);
        }
    }
    ImGui::PopStyleColor(2);

    ImGui::Dummy(ImVec2(0.0f, 4.0f));
    ImGui::BeginChild("HierarchyList", ImVec2(0.0f, 0.0f), true);
    drawEntityList();
    ImGui::EndChild();

    drawRenamePopup();
    ImGui::End();
}

void SceneHierarchyPanel::drawEntityList() {
    if (!m_hierarchyViews) {
        return;
    }

    if (m_hierarchyViews->empty()) {
        ImGui::Dummy(ImVec2(0.0f, 18.0f));
        ImGui::TextDisabled("Scene is empty");
        ImGui::TextDisabled("Add an entity or double-click an object in Content.");
        return;
    }

    for (const EditorHierarchyView& entity : *m_hierarchyViews) {
        const long id = entity.entityID;
        const bool isSelected = (m_selectedEntityID == id);

        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_SpanAvailWidth |
            ImGuiTreeNodeFlags_Leaf |
            ImGuiTreeNodeFlags_NoTreePushOnOpen |
            ImGuiTreeNodeFlags_FramePadding;
        if (isSelected) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        ImGui::PushID(id);
        ImGui::TreeNodeEx("##entity", flags, "%s", entity.name.c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            m_selectedEntityID = id;
            EditorEvent event;
            event.type = EditorEventType::GiveEntityInfo;
            event.info.entityID = id;
            EventSystem::pushEvent(event);
        }

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Rename")) {
                m_openRenamePopup = true;
                m_renamingEntityID = id;
                strncpy_s(m_renameBuffer, sizeof(m_renameBuffer), entity.name.c_str(), _TRUNCATE);
            }

            if (ImGui::MenuItem("Duplicate")) {
                EditorEvent event;
                event.type = EditorEventType::DuplicateObject;
                event.info.entityID = id;
                EventSystem::pushEvent(event);
            }

            if (ImGui::MenuItem("Delete")) {
                EditorEvent event;
                event.type = EditorEventType::DeleteObject;
                event.info.entityID = id;
                EventSystem::pushEvent(event);
            }

            ImGui::EndPopup();
        }

        ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 48.0f);
        ImGui::TextDisabled("#%ld", id);
        ImGui::PopID();
    }
}

void SceneHierarchyPanel::drawRenamePopup() {
    if (m_openRenamePopup) {
        ImGui::OpenPopup("Rename Entity");
        m_openRenamePopup = false;
    }

    if (ImGui::BeginPopupModal("Rename Entity", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("New name");
        ImGui::SetNextItemWidth(260.0f);
        ImGui::InputText("##rename", m_renameBuffer, sizeof(m_renameBuffer));

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            if (m_renamingEntityID >= 0) {
                EditorEvent event;
                event.type = EditorEventType::RenameObject;
                event.info.entityID = m_renamingEntityID;
                event.info.name = m_renameBuffer;
                EventSystem::pushEvent(event);
            }
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
