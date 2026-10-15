#include "InspectorPanel.h"

#include <cstring>
#include <string>

#include <imgui.h>

#include "Core/EventSystem/EventSystem.h"
#include "Core/Logger/Logger.h"
#include "Systems/UI/Panels/ContentBrowserPanel.h"
#include "Systems/UI/Theme/EditorTheme.h"

void InspectorPanel::setSelectedEntity(const EditorEntityView* pEntity) {
    m_pSelectedEntity = pEntity;
}

void InspectorPanel::setContentBrowserPanel(ContentBrowserPanel* pContentBrowser) {
    m_pContentBrowser = pContentBrowser;
}

void InspectorPanel::setGizmoOperation(GizmoOperation operation) {
    m_gizmoOperation = operation;
}

void InspectorPanel::draw(const EditorLayout& layout) {
    if (!m_visible) {
        return;
    }

    applyPanelRect(layout.inspector);
    ImGui::Begin("##Inspector", nullptr, EDITOR_PANEL_WINDOW_FLAGS);
    EditorTheme::drawPanelHeader("Inspector", "Properties");

    if (m_pSelectedEntity == nullptr) {
        m_lastSelectedEntityID = -1;
        ImGui::Dummy(ImVec2(0.0f, 18.0f));
        ImGui::TextDisabled("No entity selected");
        ImGui::TextDisabled("Pick one in Hierarchy to edit properties.");
        ImGui::End();
        return;
    }

    const EditorEntityView& entity = *m_pSelectedEntity;
    const long selected_entity_id = entity.entityID;

    if (m_lastSelectedEntityID != selected_entity_id || !ImGui::IsAnyItemActive()) {
        syncSelectedEntityFields(entity);
        m_lastSelectedEntityID = selected_entity_id;
    }

    ImGui::SetNextItemWidth(-1.0f);
    if (ImGui::InputText("##EntityName", m_entityNameBuffer, sizeof(m_entityNameBuffer))) {
        EditorEvent rename_event;
        rename_event.type = EditorEventType::RenameEntity;
        rename_event.info.entityID = selected_entity_id;
        rename_event.info.name = m_entityNameBuffer;
        EventSystem::pushEvent(rename_event);
    }

    ImGui::TextDisabled("ID  %ld", selected_entity_id);
    ImGui::Dummy(ImVec2(0.0f, 4.0f));

    const char* gizmo_names[] = { "Move", "Rotate", "Scale" };
    int gizmo_index = static_cast<int>(m_gizmoOperation);
    ImGui::SetNextItemWidth(-1.0f);
    if (ImGui::Combo("##Gizmo", &gizmo_index, gizmo_names, IM_ARRAYSIZE(gizmo_names))) {
        EditorEvent gizmo_event;
        if (gizmo_index == 0) {
            gizmo_event.type = EditorEventType::SetGizmoTranslate;
        }
        else if (gizmo_index == 1) {
            gizmo_event.type = EditorEventType::SetGizmoRotate;
        }
        else if (gizmo_index == 2) {
            gizmo_event.type = EditorEventType::SetGizmoScale;
        }
        EventSystem::pushEvent(gizmo_event);
    }

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        const bool position_changed = drawVec3("Position", m_position, 0.01f);
        const bool rotation_changed = drawVec3("Rotation", m_rotation, 0.1f);
        const bool scale_changed = drawVec3("Scale", m_scale, 0.01f);

        if (scale_changed) {
            for (float& scale_value : m_scale) {
                if (scale_value < 1.0f) {
                    scale_value = 1.0f;
                }
            }
        }

        auto queue_transform_event = [this, selected_entity_id](EditorEventType type) {
            EditorEvent transform_event;
            transform_event.type = type;
            transform_event.info.entityID = selected_entity_id;
            transform_event.info.translation = EditorVec3{ m_position[0], m_position[1], m_position[2] };
            transform_event.info.rotation = EditorVec3{ m_rotation[0], m_rotation[1], m_rotation[2] };
            transform_event.info.scale = EditorVec3{ m_scale[0], m_scale[1], m_scale[2] };
            EventSystem::pushEvent(transform_event);
        };

        if (position_changed) {
            queue_transform_event(EditorEventType::Translate);
        }
        if (rotation_changed) {
            queue_transform_event(EditorEventType::Rotate);
        }
        if (scale_changed) {
            queue_transform_event(EditorEventType::Scale);
        }
    }

    if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::TextDisabled("Mesh    %llu", static_cast<unsigned long long>(entity.meshID));
        ImGui::TextDisabled("Texture %llu", static_cast<unsigned long long>(entity.textureID));
        ImGui::Separator();

        float color_values[4] = {
            entity.color.r,
            entity.color.g,
            entity.color.b,
            entity.color.a
        };

        bool color_changed = ImGui::ColorEdit4("Color", color_values);
        color_changed |= ImGui::SliderFloat("Alpha", &color_values[3], 0.0f, 1.0f);

        bool is_color_tint_enabled = entity.colorEnabled;
        if (ImGui::Checkbox("Use entity color", &is_color_tint_enabled)) {
            EditorEvent tint_event;
            tint_event.type = EditorEventType::ToggleEntityColor;
            tint_event.info.entityID = selected_entity_id;
            tint_event.info.isColorTintEnabled = is_color_tint_enabled;
            EventSystem::pushEvent(tint_event);
        }

        if (color_changed) {
            for (float& component : color_values) {
                if (component < 0.0f) {
                    component = 0.0f;
                }
                else if (component > 1.0f) {
                    component = 1.0f;
                }
            }

            EditorEvent color_event;
            color_event.type = EditorEventType::ChangeEntityColor;
            color_event.info.entityID = selected_entity_id;
            color_event.info.color = Color{ color_values[0], color_values[1], color_values[2], color_values[3] };
            EventSystem::pushEvent(color_event);
        }

        const char* material_names[] = {
            "Default", "Unlit", "PBR Standard", "Blinn-Phong",
            "Glass", "Water", "Terrain", "Skybox", "Custom"
        };
        m_currentMaterial = static_cast<int>(entity.shaderID);
        ImGui::SetNextItemWidth(-1.0f);
        if (ImGui::Combo("Material", &m_currentMaterial, material_names, IM_ARRAYSIZE(material_names))) {
            EditorEvent material_event;
            material_event.type = EditorEventType::SetEntityMaterial;
            material_event.info.entityID = selected_entity_id;
            material_event.info.materialType = static_cast<MaterialType>(m_currentMaterial);
            EventSystem::pushEvent(material_event);
        }
    }

    if (ImGui::CollapsingHeader("Shader")) {
        ImGui::TextDisabled("Program is selected by material type");
    }

    ImGui::Dummy(ImVec2(0.0f, 6.0f));
    if (ImGui::Button("Add Component", ImVec2(-1.0f, 0.0f))) {
        ImGui::OpenPopup("AddComponentPopup");
    }

    if (ImGui::BeginPopup("AddComponentPopup")) {
        if (ImGui::MenuItem("Texture")) {
            if (m_pContentBrowser != nullptr) {
                Logger::addLog(
                    LogType::LOG_INFO,
                    "Texture selection mode activated for Entity ID: " + std::to_string(selected_entity_id));
                m_pContentBrowser->setTextureSelectionMode(true, selected_entity_id);
            }
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

void InspectorPanel::syncSelectedEntityFields(const EditorEntityView& entity) {
    strncpy_s(m_entityNameBuffer, sizeof(m_entityNameBuffer), entity.name.c_str(), _TRUNCATE);

    m_position[0] = entity.position.x;
    m_position[1] = entity.position.y;
    m_position[2] = entity.position.z;

    m_rotation[0] = entity.rotation.x;
    m_rotation[1] = entity.rotation.y;
    m_rotation[2] = entity.rotation.z;

    m_scale[0] = entity.scale.x;
    m_scale[1] = entity.scale.y;
    m_scale[2] = entity.scale.z;
}

bool InspectorPanel::drawVec3(const char* label, float values[3], float speed) {
    ImGui::PushID(label);
    ImGui::TextUnformatted(label);

    const char* axis_labels[] = { "X", "Y", "Z" };
    const ImVec4 axis_colors[] = {
        ImVec4(0.78f, 0.28f, 0.28f, 1.0f),
        ImVec4(0.32f, 0.68f, 0.36f, 1.0f),
        ImVec4(0.28f, 0.48f, 0.86f, 1.0f)
    };

    bool changed = false;
    const float item_width = (ImGui::GetContentRegionAvail().x - 16.0f) / 3.0f;
    for (int i = 0; i < 3; ++i) {
        ImGui::PushID(i);
        ImGui::PushStyleColor(ImGuiCol_Button, axis_colors[i]);
        ImGui::Button(axis_labels[i], ImVec2(22.0f, 0.0f));
        ImGui::PopStyleColor();
        ImGui::SameLine(0.0f, 4.0f);
        ImGui::SetNextItemWidth(item_width - 26.0f);
        changed |= ImGui::DragFloat("##v", &values[i], speed);
        ImGui::PopID();
        if (i < 2) {
            ImGui::SameLine();
        }
    }

    ImGui::PopID();
    return changed;
}
