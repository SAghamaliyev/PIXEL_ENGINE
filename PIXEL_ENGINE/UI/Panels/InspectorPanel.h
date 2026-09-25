#pragma once

#include "../Layout/EditorLayout.h"
#include "../../Core/EventSystem/EventTypes.h"

#include <vector>

class ContentBrowserPanel;

class InspectorPanel {
public:
    void setSelectedEntity(const EditorEntityView* entity);
    void setContentBrowserPanel(ContentBrowserPanel* contentBrowser);
    void setGizmoOperation(EditorGizmoOperation operation);

    void draw(const EditorLayout& layout);

    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

private:
    const EditorEntityView* m_selectedEntity = nullptr;
    ContentBrowserPanel* m_contentBrowser = nullptr;
    bool m_visible = true;
    long m_lastSelectedEntityID = -1;

    char m_entityNameBuffer[128] = "";
    float m_position[3] = { 0.0f, 0.0f, 0.0f };
    float m_rotation[3] = { 0.0f, 0.0f, 0.0f };
    float m_scale[3] = { 1.0f, 1.0f, 1.0f };
    int m_currentMaterial = 0;
    EditorGizmoOperation m_gizmoOperation = EditorGizmoOperation::Translate;

    void syncSelectedEntityFields(const EditorEntityView& entity);
    bool drawVec3(const char* label, float values[3], float speed);
};
