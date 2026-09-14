#include "InspectorPanel.h"
#include "ContentBrowserPanel.h"

#include "../Materials/imgui.h"
#include "../../Logger/Logger.h"

#include <cstring>

void InspectorPanel::setEventQueue(std::vector<EditorEvent>* events) {
    m_events = events;
}

void InspectorPanel::setEntityViews(const std::vector<EditorEntityView>* entityViews) {
    m_entityViews = entityViews;
}

void InspectorPanel::setSelectedEntityID(int entityID) {
    m_selectedEntityID = entityID;
}

void InspectorPanel::setContentBrowserPanel(ContentBrowserPanel* contentBrowser) {
    m_contentBrowser = contentBrowser;
}

void InspectorPanel::draw(const EditorLayout& layout) {
    if (!m_visible) {
        return;
    }

    applyPanelRect(layout.inspector);
    ImGui::Begin("Inspector", nullptr, kEditorPanelWindowFlags);

    const EditorEntityView* entity = findSelectedEntity();
    if (m_selectedEntityID < 0 || !entity) {
        ImGui::TextDisabled("No entity selected");
        ImGui::End();
        return;
    }

    static int lastSelectedID = -1;
    if (lastSelectedID != m_selectedEntityID) {
        strncpy_s(m_entityNameBuffer, sizeof(m_entityNameBuffer), entity->name.c_str(), _TRUNCATE);
        lastSelectedID = m_selectedEntityID;
    }

    if (ImGui::InputText("##EntityName", m_entityNameBuffer, sizeof(m_entityNameBuffer))) {
        // /FLAG RenameObject: UI requests renaming the selected entity.
        EditorEvent event;
        event.type = EditorEventType::RenameObject;
        event.entityID = (unsigned int)m_selectedEntityID;
        event.name = m_entityNameBuffer;
        pushEvent(event);
    }

    ImGui::Text("Entity ID: %d", m_selectedEntityID);
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        const bool positionChanged = ImGui::DragFloat3("Position", m_position, 0.01f);
        const bool rotationChanged = ImGui::DragFloat3("Rotation", m_rotation, 0.1f);
        const bool scaleChanged = ImGui::DragFloat3("Scale", m_scale, 0.01f);

        auto queueTransformEvent = [this](EditorEventType type) {
            EditorEvent event;
            event.type = type;
            event.entityID = (unsigned int)m_selectedEntityID;
            event.position = EditorVec3{ m_position[0], m_position[1], m_position[2] };
            event.rotation = EditorVec3{ m_rotation[0], m_rotation[1], m_rotation[2] };
            event.scale = EditorVec3{ m_scale[0], m_scale[1], m_scale[2] };
            pushEvent(event);
        };

        if (positionChanged) {
            // /FLAG Translate: UI requests a translation for the selected entity.
            queueTransformEvent(EditorEventType::Translate);
        }
        if (rotationChanged) {
            // /FLAG Rotate: UI requests a rotation for the selected entity.
            queueTransformEvent(EditorEventType::Rotate);
        }
        if (scaleChanged) {
            // /FLAG Scale: UI requests a scale change for the selected entity.
            queueTransformEvent(EditorEventType::Scale);
        }
    }

    if (ImGui::CollapsingHeader("MeshRenderer", ImGuiTreeNodeFlags_DefaultOpen)) {
        char meshIDText[256] = "";
        const std::string meshID = std::to_string(entity->meshID);
        strncpy_s(meshIDText, sizeof(meshIDText), meshID.c_str(), _TRUNCATE);
        ImGui::InputText("Mesh ID", meshIDText, sizeof(meshIDText), ImGuiInputTextFlags_ReadOnly);

        char textureIDText[256] = "";
        const std::string textureID = std::to_string(entity->textureID);
        strncpy_s(textureIDText, sizeof(textureIDText), textureID.c_str(), _TRUNCATE);
        ImGui::InputText("Texture ID", textureIDText, sizeof(textureIDText), ImGuiInputTextFlags_ReadOnly);

        ImGui::Separator();

        float color[4] = {
            entity->color.r,
            entity->color.g,
            entity->color.b,
            entity->color.a
        };

        bool colorChanged = ImGui::ColorEdit4("Color (RGBA)", color);
        colorChanged |= ImGui::SliderFloat("Alpha", &color[3], 0.0f, 1.0f);

        bool colorEnabled = entity->colorEnabled;
        if (ImGui::Checkbox("Enable Entity Color", &colorEnabled)) {
            // /FLAG ToggleEntityColor: UI requests enabling or disabling the entity color.
            EditorEvent event;
            event.type = EditorEventType::ToggleEntityColor;
            event.entityID = (unsigned int)m_selectedEntityID;
            event.colorEnabled = colorEnabled;
            pushEvent(event);
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

            // /FLAG ChangeEntityColor: UI requests a color update for the selected entity.
            EditorEvent event;
            event.type = EditorEventType::ChangeEntityColor;
            event.entityID = (unsigned int)m_selectedEntityID;
            event.color = Color{ color[0], color[1], color[2], color[3] };
            pushEvent(event);
        }

        const char* materialNames[] = {
            "Default", "Unlit", "PBR_Standard", "BlinnPhong",
            "Glass", "Water", "Terrain", "Skybox", "Custom"
        };
        m_currentMaterial = (int)entity->ShaderID;
        if (ImGui::Combo("Material", &m_currentMaterial, materialNames, IM_ARRAYSIZE(materialNames))) {
            // /FLAG ChangeEntityMaterial: UI requests a material change for the selected entity.
            EditorEvent event;
            event.type = EditorEventType::ChangeEntityMaterial;
            event.entityID = (unsigned int)m_selectedEntityID;
            event.ShaderID = (MaterialType)m_currentMaterial;
            pushEvent(event);
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
        auto queueComponentEvent = [this](EditorComponentType componentType) {
            EditorEvent event;
            event.type = EditorEventType::AddComponent;
            event.entityID = (unsigned int)m_selectedEntityID;
            event.componentType = componentType;
            pushEvent(event);
        };

        if (ImGui::MenuItem("RigidBody")) {
            // /FLAG AddComponent: UI requests adding a RigidBody component.
            queueComponentEvent(EditorComponentType::RigidBody);
        }
        if (ImGui::MenuItem("Script")) {
            // /FLAG AddComponent: UI requests adding a Script component.
            queueComponentEvent(EditorComponentType::Script);
        }
        if (ImGui::MenuItem("Collider")) {
            // /FLAG AddComponent: UI requests adding a Collider component.
            queueComponentEvent(EditorComponentType::Collider);
        }
        if (ImGui::MenuItem("Light")) {
            // /FLAG AddComponent: UI requests adding a Light component.
            queueComponentEvent(EditorComponentType::Light);
        }
        if (ImGui::MenuItem("Camera")) {
            // /FLAG AddComponent: UI requests adding a Camera component.
            queueComponentEvent(EditorComponentType::Camera);
        }
        if (ImGui::MenuItem("Audio Source")) {
            // /FLAG AddComponent: UI requests adding an AudioSource component.
            queueComponentEvent(EditorComponentType::AudioSource);
        }
        if (ImGui::MenuItem("Texture")) {
            // /FLAG AssignTexture: UI requests texture selection for the entity.
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

void InspectorPanel::pushEvent(const EditorEvent& event) {
    if (m_events) {
        // /FLAG Stores the event flag for the engine-side event processor.
        m_events->push_back(event);
    }
}
