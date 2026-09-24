#include "InspectorPanel.h"
#include "ContentBrowserPanel.h"

#include "../Theme/EditorTheme.h"
#include "../Materials/imgui.h"
#include "../../Core/EventSystem/EventSystem.h"
#include "../../Core/Logger/Logger.h"

#include <cstring>
#include <string>

void InspectorPanel::setEntityViews(const std::vector<EditorEntityView>* entityViews) {
    m_entityViews = entityViews;
}

void InspectorPanel::setSelectedEntityID(int entityID) {
    m_selectedEntityID = entityID;
}

void InspectorPanel::setContentBrowserPanel(ContentBrowserPanel* contentBrowser) {
    m_contentBrowser = contentBrowser;
}

void InspectorPanel::setGizmoOperation(EditorGizmoOperation operation) {
    m_gizmoOperation = operation;
}

void InspectorPanel::draw(const EditorLayout& layout) {
    if (!m_visible) {
        return;
    }

    applyPanelRect(layout.inspector);
    ImGui::Begin("##Inspector", nullptr, kEditorPanelWindowFlags);
    EditorTheme::drawPanelHeader("Inspector", "Properties");

    const EditorEntityView* entity = findSelectedEntity();
    if (m_selectedEntityID < 0 || !entity) {
        m_lastSelectedEntityID = -1;
        ImGui::Dummy(ImVec2(0.0f, 18.0f));
        ImGui::TextDisabled("No entity selected");
        ImGui::TextDisabled("Pick one in Hierarchy to edit properties.");
        ImGui::End();
        return;
    }

    if (m_lastSelectedEntityID != m_selectedEntityID || !ImGui::IsAnyItemActive()) {
        syncSelectedEntityFields(*entity);
        m_lastSelectedEntityID = m_selectedEntityID;
    }

    ImGui::SetNextItemWidth(-1.0f);
    if (ImGui::InputText("##EntityName", m_entityNameBuffer, sizeof(m_entityNameBuffer))) {
        EditorEvent event;
        event.type = EditorEventType::RenameObject;
        event.info.entityID = (unsigned int)m_selectedEntityID;
        event.info.name = m_entityNameBuffer;
        EventSystem::pushEvent(event);
    }

    ImGui::TextDisabled("ID  %d", m_selectedEntityID);
    ImGui::Dummy(ImVec2(0.0f, 4.0f));

    const char* gizmoNames[] = { "Move", "Rotate", "Scale" };
    int gizmoIndex = (int)m_gizmoOperation;
    ImGui::SetNextItemWidth(-1.0f);
    if (ImGui::Combo("##Gizmo", &gizmoIndex, gizmoNames, IM_ARRAYSIZE(gizmoNames))) {
        EditorEvent event;
        event.type = EditorEventType::SetGizmoOperation;
        event.info.gizmoOperation = (EditorGizmoOperation)gizmoIndex;
        EventSystem::pushEvent(event);
    }

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        const bool positionChanged = drawVec3("Position", m_position, 0.01f);
        const bool rotationChanged = drawVec3("Rotation", m_rotation, 0.1f);
        const bool scaleChanged = drawVec3("Scale", m_scale, 0.01f);

        if (scaleChanged) {
            for (float& scaleValue : m_scale) {
                if (scaleValue < 1.0f) {
                    scaleValue = 1.0f;
                }
            }
        }

        auto queueTransformEvent = [this](EditorEventType type) {
            EditorEvent event;
            event.type = type;
            event.info.entityID = (unsigned int)m_selectedEntityID;
            event.info.position = EditorVec3{ m_position[0], m_position[1], m_position[2] };
            event.info.rotation = EditorVec3{ m_rotation[0], m_rotation[1], m_rotation[2] };
            event.info.scale = EditorVec3{ m_scale[0], m_scale[1], m_scale[2] };
            EventSystem::pushEvent(event);
        };

        if (positionChanged) {
            queueTransformEvent(EditorEventType::Translate);
        }
        if (rotationChanged) {
            queueTransformEvent(EditorEventType::Rotate);
        }
        if (scaleChanged) {
            queueTransformEvent(EditorEventType::Scale);
        }
    }

    if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::TextDisabled("Mesh    %llu", (unsigned long long)entity->meshID);
        ImGui::TextDisabled("Texture %llu", (unsigned long long)entity->textureID);
        ImGui::Separator();

        float color[4] = {
            entity->color.r,
            entity->color.g,
            entity->color.b,
            entity->color.a
        };

        bool colorChanged = ImGui::ColorEdit4("Color", color);
        colorChanged |= ImGui::SliderFloat("Alpha", &color[3], 0.0f, 1.0f);

        bool colorEnabled = entity->colorEnabled;
        if (ImGui::Checkbox("Use entity color", &colorEnabled)) {
            EditorEvent event;
            event.type = EditorEventType::ToggleEntityColor;
            event.info.entityID = (unsigned int)m_selectedEntityID;
            event.info.colorEnabled = colorEnabled;
            EventSystem::pushEvent(event);
        }

        if (colorChanged) {
            for (float& component : color) {
                if (component < 0.0f) {
                    component = 0.0f;
                }
                else if (component > 1.0f) {
                    component = 1.0f;
                }
            }

            EditorEvent event;
            event.type = EditorEventType::ChangeEntityColor;
            event.info.entityID = (unsigned int)m_selectedEntityID;
            event.info.color = Color{ color[0], color[1], color[2], color[3] };
            EventSystem::pushEvent(event);
        }

        const char* materialNames[] = {
            "Default", "Unlit", "PBR Standard", "Blinn-Phong",
            "Glass", "Water", "Terrain", "Skybox", "Custom"
        };
        m_currentMaterial = (int)entity->ShaderID;
        ImGui::SetNextItemWidth(-1.0f);
        if (ImGui::Combo("Material", &m_currentMaterial, materialNames, IM_ARRAYSIZE(materialNames))) {
            EditorEvent event;
            event.type = EditorEventType::ChangeEntityMaterial;
            event.info.entityID = (unsigned int)m_selectedEntityID;
            event.info.ShaderID = (MaterialType)m_currentMaterial;
            EventSystem::pushEvent(event);
        }
    }

    if (ImGui::CollapsingHeader("Shader")) {
        ImGui::TextDisabled("Program is selected by material type");
    }

    ImGui::Dummy(ImVec2(0.0f, 6.0f));
    if (ImGui::Button("Add Component", ImVec2(-1, 0))) {
        ImGui::OpenPopup("AddComponentPopup");
    }

    if (ImGui::BeginPopup("AddComponentPopup")) {
        auto queueComponentEvent = [this](EditorComponentType componentType) {
            EditorEvent event;
            event.type = EditorEventType::AddComponent;
            event.info.entityID = (unsigned int)m_selectedEntityID;
            event.info.componentType = componentType;
            EventSystem::pushEvent(event);
        };

        if (ImGui::MenuItem("RigidBody")) {
            queueComponentEvent(EditorComponentType::RigidBody);
        }
        if (ImGui::MenuItem("Script")) {
            queueComponentEvent(EditorComponentType::Script);
        }
        if (ImGui::MenuItem("Collider")) {
            queueComponentEvent(EditorComponentType::Collider);
        }
        if (ImGui::MenuItem("Light")) {
            queueComponentEvent(EditorComponentType::Light);
        }
        if (ImGui::MenuItem("Camera")) {
            queueComponentEvent(EditorComponentType::Camera);
        }
        if (ImGui::MenuItem("Audio Source")) {
            queueComponentEvent(EditorComponentType::AudioSource);
        }
        if (ImGui::MenuItem("Texture")) {
            if (m_contentBrowser) {
                Logger::addLog(LOG_INFO,
                    "Texture selection mode activated for Entity ID: " + std::to_string(m_selectedEntityID));
                m_contentBrowser->setTextureSelectionMode(true, (unsigned int)m_selectedEntityID);
            }
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

const EditorEntityView* InspectorPanel::findSelectedEntity() const {
    if (!m_entityViews || m_selectedEntityID < 0) {
        return nullptr;
    }

    for (const EditorEntityView& entity : *m_entityViews) {
        if (entity.entityID == (unsigned int)m_selectedEntityID) {
            return &entity;
        }
    }

    return nullptr;
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
    ImGui::TextUnformatted(label);

    const char* axisLabels[] = { "X", "Y", "Z" };
    const ImVec4 axisColors[] = {
        ImVec4(0.78f, 0.28f, 0.28f, 1.0f),
        ImVec4(0.32f, 0.68f, 0.36f, 1.0f),
        ImVec4(0.28f, 0.48f, 0.86f, 1.0f)
    };

    bool changed = false;
    const float itemWidth = (ImGui::GetContentRegionAvail().x - 16.0f) / 3.0f;
    for (int i = 0; i < 3; ++i) {
        ImGui::PushID(i);
        ImGui::PushStyleColor(ImGuiCol_Button, axisColors[i]);
        ImGui::Button(axisLabels[i], ImVec2(22.0f, 0.0f));
        ImGui::PopStyleColor();
        ImGui::SameLine(0.0f, 4.0f);
        ImGui::SetNextItemWidth(itemWidth - 26.0f);
        changed |= ImGui::DragFloat("##v", &values[i], speed);
        ImGui::PopID();
        if (i < 2) {
            ImGui::SameLine();
        }
    }

    return changed;
}
