#pragma once

#include "../EditorLayout.h"

class SceneSystem;

class InspectorPanel {
public:
    void setSceneSystem(SceneSystem* sceneSystem);
    void setSelectedEntityID(int entityID);

    void draw(const EditorLayout& layout);

    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

private:
    SceneSystem* m_sceneSystem = nullptr;
    bool m_visible = true;
    int m_selectedEntityID = -1;

    char m_entityNameBuffer[128] = "";
    float m_position[3] = { 0.0f, 0.0f, 0.0f };
    float m_rotation[3] = { 0.0f, 0.0f, 0.0f };
    float m_scale[3] = { 1.0f, 1.0f, 1.0f };
    int m_currentMaterial = 0;
};
