#include "InspectorPanel.h"

#include "../../SceneSystem/SceneSystem.h"
#include "../Materials/imgui.h"

#include <cstring>

void InspectorPanel::setSceneSystem(SceneSystem* sceneSystem) {
    m_sceneSystem = sceneSystem;
}

void InspectorPanel::setSelectedEntityID(int entityID) {
    m_selectedEntityID = entityID;
}

void InspectorPanel::draw(const EditorLayout& layout) {
    if (!m_visible) {
        return;
    }

    applyPanelRect(layout.inspector);
    ImGui::Begin("Inspector", nullptr, kEditorPanelWindowFlags);

    if (m_selectedEntityID < 0 || !m_sceneSystem || !m_sceneSystem->SceneHasEntity((unsigned int)m_selectedEntityID)) {
        ImGui::TextDisabled("No entity selected");
        ImGui::End();
        return;
    }

    EntityUnit& entity = m_sceneSystem->SceneGetEntity((unsigned int)m_selectedEntityID);

    static int lastSelectedID = -1;
    if (lastSelectedID != m_selectedEntityID) {
        strncpy_s(m_entityNameBuffer, sizeof(m_entityNameBuffer), entity.name.c_str(), _TRUNCATE);
        lastSelectedID = m_selectedEntityID;
    }

    if (ImGui::InputText("##EntityName", m_entityNameBuffer, sizeof(m_entityNameBuffer))) {
        // TODO: apply the edited name through SceneSystem. DONE!!!!

        m_sceneSystem->SceneRenameEntity((unsigned int)m_selectedEntityID, m_entityNameBuffer);
    
    }

    ImGui::Text("Entity ID: %d", m_selectedEntityID);
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::DragFloat3("Position", m_position, 0.01f);
        // TODO: bind Position to the entity transform when SceneSystem exposes it.
        // Example: entity.transform.position = glm::vec3(m_position[0], m_position[1], m_position[2]);

        ImGui::DragFloat3("Rotation", m_rotation, 0.1f);
        // TODO: bind Rotation to the entity transform.

        ImGui::DragFloat3("Scale", m_scale, 0.01f);
        // TODO: bind Scale to the entity transform.
    }

    if (ImGui::CollapsingHeader("MeshRenderer", ImGuiTreeNodeFlags_DefaultOpen)) {
        char meshPath[256];
        strncpy_s(meshPath, sizeof(meshPath), entity.path.c_str(), _TRUNCATE);
        ImGui::InputText("Model", meshPath, sizeof(meshPath), ImGuiInputTextFlags_ReadOnly);
        // TODO: add a "..." button to pick another mesh through the Content Browser / AssetSystem.

        ImGui::Separator();

        const char* materialNames[] = {
            "Default", "Unlit", "PBR_Standard", "BlinnPhong",
            "Glass", "Water", "Terrain", "Skybox", "Custom"
        };
        m_currentMaterial = (int)entity.MaterialID;
        if (ImGui::Combo("Material", &m_currentMaterial, materialNames, IM_ARRAYSIZE(materialNames))) {
            // TODO: apply the selected material through SceneSystem.
            /*
                Example:
                m_sceneSystem->SceneSetEntityMaterial(
                    (unsigned int)m_selectedEntityID,
                    (MaterialType)m_currentMaterial
                );
            */
        }
    }

    if (ImGui::CollapsingHeader("Shader", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Shader Program ID: ---");
        ImGui::TextDisabled("(determined by material type)");
    }

    ImGui::Separator();

    if (ImGui::Button("Add Component", ImVec2(-1, 0))) {
        ImGui::OpenPopup("AddComponentPopup");
    }

    if (ImGui::BeginPopup("AddComponentPopup")) {
        if (ImGui::MenuItem("RigidBody")) {
            // TODO: add a RigidBody component through SceneSystem.
        }
        if (ImGui::MenuItem("Script")) {
            // TODO: add a Script component through SceneSystem.
        }
        if (ImGui::MenuItem("Collider")) {
            // TODO: add a Collider component through SceneSystem.
        }
        if (ImGui::MenuItem("Light")) {
            // TODO: add a Light component through SceneSystem.
        }
        if (ImGui::MenuItem("Camera")) {
            // TODO: add a Camera component through SceneSystem.
        }
        if (ImGui::MenuItem("Audio Source")) {
            // TODO: add an AudioSource component through SceneSystem.
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}
