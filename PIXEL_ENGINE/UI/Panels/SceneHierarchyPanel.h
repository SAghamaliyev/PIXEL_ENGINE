#pragma once

#include "../Layout/EditorLayout.h"
#include "../../Core/EventSystem/EventTypes.h"

#include <vector>

class SceneHierarchyPanel {
public:
    void setHierarchyViews(const std::vector<EditorHierarchyView>* hierarchyViews);

    void draw(const EditorLayout& layout);

    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

    long getSelectedEntityID() const { return m_selectedEntityID; }
    void setSelectedEntityID(long entityID) { m_selectedEntityID = entityID; }

private:
    const std::vector<EditorHierarchyView>* m_hierarchyViews = nullptr;
    bool m_visible = true;
    long m_selectedEntityID = -1;

    char m_renameBuffer[128] = "";
    long m_renamingEntityID = -1;
    bool m_openRenamePopup = false;

    void drawEntityList();
    void drawRenamePopup();
};
