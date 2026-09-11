#include "SceneHierarchyPanel.h"

#include "../Materials/imgui.h"
#include "../../Logger/Logger.h"

#include <cstring>

void SceneHierarchyPanel::setEventQueue(std::vector<EditorEvent>* events) {
    m_events = events;
}

void SceneHierarchyPanel::setEntityViews(const std::vector<EditorEntityView>* entityViews) {
    m_entityViews = entityViews;
}

void SceneHierarchyPanel::draw(const EditorLayout& layout) {
    if (!m_visible) {
        return;
    }

    applyPanelRect(layout.hierarchy);
    ImGui::Begin("Scene Hierarchy", nullptr, kEditorPanelWindowFlags);

    if (ImGui::Button("Add Entity")) {
        EditorEvent event;
        event.type = EditorEventType::AddObject;
        event.isEmptyEntity = true;
        pushEvent(event);
        Logger::addLog(LOG_INFO, "Add Entity event queued.");
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete")) {
        if (m_selectedEntityID >= 0) {
            // /FLAG DeleteObject: UI requests deleting the selected entity.
            pushEvent(EditorEvent{ EditorEventType::DeleteObject, (unsigned int)m_selectedEntityID });
            m_selectedEntityID = -1;
        }
    }

    drawEntityList();
    drawRenamePopup();

    ImGui::End();
}

void SceneHierarchyPanel::drawEntityList() {
    if (!m_entityViews) {
        return;
    }

    if (m_entityViews->empty()) {
        ImGui::TextDisabled("(empty - add entities via Add Entity or Content Browser)");
        return;
    }

    for (const EditorEntityView& entity : *m_entityViews) {
        const unsigned int id = entity.entityID;
        const bool isSelected = (m_selectedEntityID == (int)id);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (isSelected) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        const bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)id, flags, "%s", entity.name.c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            m_selectedEntityID = (int)id;
        }

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Rename")) {
                m_openRenamePopup = true;
                m_renamingEntityID = (int)id;
                strncpy_s(m_renameBuffer, sizeof(m_renameBuffer), entity.name.c_str(), _TRUNCATE);
            }

            if (ImGui::MenuItem("Delete")) {
                // /FLAG DeleteObject: UI requests deleting the context-menu entity.
                pushEvent(EditorEvent{ EditorEventType::DeleteObject, id });
                if (m_selectedEntityID == (int)id) {
                    m_selectedEntityID = -1;
                }
            }

            if (ImGui::MenuItem("Duplicate")) {
                EditorEvent event;
                event.type = EditorEventType::DuplicateObject;
                event.entityID = id;
                pushEvent(event);
            }

            ImGui::EndPopup();
        }

        if (nodeOpen) {
            ImGui::TreePop();
        }
    }
}

void SceneHierarchyPanel::drawRenamePopup() {
    if (m_openRenamePopup) {
        ImGui::OpenPopup("Rename Entity");
        m_openRenamePopup = false;
    }

    if (ImGui::BeginPopupModal("Rename Entity", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("New name:");
        ImGui::InputText("##rename", m_renameBuffer, sizeof(m_renameBuffer));

        if (ImGui::Button("OK", ImVec2(120, 0))) {

            if (m_renamingEntityID >= 0) {
                // /FLAG RenameObject: UI requests renaming an entity from the popup input.
                EditorEvent event;
                event.type = EditorEventType::RenameObject;
                event.entityID = (unsigned int)m_renamingEntityID;
                event.name = m_renameBuffer;
                pushEvent(event);
            }

            m_renamingEntity = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            m_renamingEntity = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void SceneHierarchyPanel::pushEvent(const EditorEvent& event) {
    if (m_events) {
        // /FLAG Stores the event flag for the engine-side event processor.
        m_events->push_back(event);
    }
}
