#include "SceneHierarchyPanel.h"

#include <cstring>
#include <string>
#include <vector>

#include <imgui.h>

#include "Core/EventSystem/EventSystem.h"
#include "Core/Logger/Logger.h"
#include "Systems/UI/Theme/EditorTheme.h"

void SceneHierarchyPanel::setHierarchyViews(const std::vector<EditorHierarchyView>* pHierarchyViews) {
    m_pHierarchyViews = pHierarchyViews;
}

void SceneHierarchyPanel::draw(const EditorLayout& layout) {
    if (!m_visible) {
        return;
    }

    applyPanelRect(layout.hierarchy);
    ImGui::Begin("##SceneHierarchy", nullptr, EDITOR_PANEL_WINDOW_FLAGS);
    EditorTheme::drawPanelHeader("Hierarchy", "Scene");

    const float add_button_width = ImGui::GetContentRegionAvail().x * 0.58f;
    if (ImGui::Button("Add Entity", ImVec2(add_button_width, 0.0f))) {
        EditorEvent add_event;
        add_event.type = EditorEventType::AddEntity;
        add_event.info.isEmptyEntity = true;
        EventSystem::pushEvent(add_event);
        Logger::addLog(LogType::LOG_INFO, "Add Entity event queued.");
    }

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.45f, 0.18f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.62f, 0.24f, 0.26f, 1.0f));
    if (ImGui::Button("Delete", ImVec2(-1.0f, 0.0f))) {
        if (m_selectedEntityID >= 0) {
            EditorEvent delete_event;
            delete_event.type = EditorEventType::DeleteEntity;
            delete_event.info.entityID = m_selectedEntityID;
            EventSystem::pushEvent(delete_event);
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
    if (m_pHierarchyViews == nullptr) {
        return;
    }

    if (m_pHierarchyViews->empty()) {
        ImGui::Dummy(ImVec2(0.0f, 18.0f));
        ImGui::TextDisabled("Scene is empty");
        ImGui::TextDisabled("Add an entity or double-click an object in Content.");
        return;
    }

    for (const EditorHierarchyView& entity_view : *m_pHierarchyViews) {
        const long id = entity_view.entityID;
        const bool is_selected = (m_selectedEntityID == id);

        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_SpanAvailWidth |
            ImGuiTreeNodeFlags_Leaf |
            ImGuiTreeNodeFlags_NoTreePushOnOpen |
            ImGuiTreeNodeFlags_FramePadding;
        if (is_selected) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        ImGui::PushID(static_cast<int>(id));
        ImGui::TreeNodeEx("##entity", flags, "%s", entity_view.name.c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            m_selectedEntityID = id;
            EditorEvent select_event;
            select_event.type = EditorEventType::RequestEntityInfo;
            select_event.info.entityID = id;
            EventSystem::pushEvent(select_event);
        }

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Rename")) {
                m_openRenamePopup = true;
                m_renamingEntityID = id;
                strncpy_s(m_renameBuffer, sizeof(m_renameBuffer), entity_view.name.c_str(), _TRUNCATE);
            }

            if (ImGui::MenuItem("Duplicate")) {
                EditorEvent dup_event;
                dup_event.type = EditorEventType::DuplicateEntity;
                dup_event.info.entityID = id;
                EventSystem::pushEvent(dup_event);
            }

            if (ImGui::MenuItem("Delete")) {
                EditorEvent del_event;
                del_event.type = EditorEventType::DeleteEntity;
                del_event.info.entityID = id;
                EventSystem::pushEvent(del_event);
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

        if (ImGui::Button("OK", ImVec2(120.0f, 0.0f))) {
            if (m_renamingEntityID >= 0) {
                EditorEvent rename_event;
                rename_event.type = EditorEventType::RenameEntity;
                rename_event.info.entityID = m_renamingEntityID;
                rename_event.info.name = m_renameBuffer;
                EventSystem::pushEvent(rename_event);
            }
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120.0f, 0.0f))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
