#pragma once

#include "../EditorLayout.h"
#include "../../EventSystem/EventSystem.h"

#include <vector>

class ConsolePanel;
class ContentBrowserPanel;

class InspectorPanel {
public:
    void setEntityViews(const std::vector<EditorEntityView>* entityViews);
    void setSelectedEntityID(int entityID);
    void setContentBrowserPanel(ContentBrowserPanel* contentBrowser);

    void draw(const EditorLayout& layout);

    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

private:
    const std::vector<EditorEntityView>* m_entityViews = nullptr;
    ContentBrowserPanel* m_contentBrowser = nullptr;
    bool m_visible = true;
    int m_selectedEntityID = -1;

    char m_entityNameBuffer[128] = "";
    float m_position[3] = { 0.0f, 0.0f, 0.0f };
    float m_rotation[3] = { 0.0f, 0.0f, 0.0f };
    float m_scale[3] = { 1.0f, 1.0f, 1.0f };
    int m_currentMaterial = 0;

    const EditorEntityView* findSelectedEntity() const;
    void pushEvent(const EditorEvent& event);
};
