#pragma once

#include "../EditorLayout.h"

class SceneSystem;

class SceneHierarchyPanel {
public:
    void setSceneSystem(SceneSystem* sceneSystem);

    void draw(const EditorLayout& layout);

    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

    int getSelectedEntityID() const { return m_selectedEntityID; }
    void setSelectedEntityID(int entityID) { m_selectedEntityID = entityID; }

private:
    SceneSystem* m_sceneSystem = nullptr;
    bool m_visible = true;
    int m_selectedEntityID = -1;

    char m_renameBuffer[128] = "";
    bool m_renamingEntity = false;
    int m_renamingEntityID = -1;
    bool m_openRenamePopup = false;

    void drawEntityList();
    void drawRenamePopup();
};
