#include "SceneHierarchyPanel.h"

#include "../../SceneSystem/SceneSystem.h"
#include "../Materials/imgui.h"

#include <cstring>

void SceneHierarchyPanel::setSceneSystem(SceneSystem* sceneSystem) {
    m_sceneSystem = sceneSystem;
}

void SceneHierarchyPanel::draw(const EditorLayout& layout) {
    if (!m_visible) {
        return;
    }

    applyPanelRect(layout.hierarchy);
    ImGui::Begin("Scene Hierarchy", nullptr, kEditorPanelWindowFlags);

    if (ImGui::Button("Add Entity")) {
        if (m_sceneSystem) {
            m_sceneSystem->SceneCreateEntity("src/Objects/Triangle.obj", Default, "NewObject");
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete")) {
        if (m_sceneSystem && m_selectedEntityID >= 0) {
            m_sceneSystem->SceneDeleteEntity((unsigned int)m_selectedEntityID);
            m_selectedEntityID = -1;
        }
    }

    drawEntityList();
    drawRenamePopup();

    ImGui::End();
}

void SceneHierarchyPanel::drawEntityList() {
    if (!m_sceneSystem) {
        return;
    }

    const auto& entityList = m_sceneSystem->getSceneInfo().EntityList;
    if (entityList.empty()) {
        ImGui::TextDisabled("(empty - add entities via Add Entity or Content Browser)");
        return;
    }

    for (const auto& pair : entityList) {
        const EntityUnit& entity = pair.second;
        const unsigned int id = entity.EntityID;
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
                m_sceneSystem->SceneDeleteEntity(id);
                if (m_selectedEntityID == (int)id) {
                    m_selectedEntityID = -1;
                }
            }

            if (ImGui::MenuItem("Duplicate")) {
                m_sceneSystem->SceneCreateEntity(entity.path, entity.MaterialID, entity.name + "_copy");
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

            if (m_sceneSystem && m_renamingEntityID >= 0) {

                m_sceneSystem->SceneRenameEntity((unsigned int)m_renamingEntityID,
                                                                 m_renameBuffer);
            }
            // TODO: apply the new name through SceneSystem DONE!!!!

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
