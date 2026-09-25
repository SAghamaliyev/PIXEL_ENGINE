#pragma once

#include "../Layout/EditorLayout.h"
#include "../../Core/EventSystem/EventTypes.h"

#include <string>
#include <vector>

class ContentBrowserPanel {
public:
    void draw(const EditorLayout& layout);

    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

    void setTextureSelectionMode(bool enabled, long entityID = 0);

private:
    bool m_visible = true;

    bool m_textureSelectionMode = false;
    long m_selectedEntityIDForTexture = 0;

    char m_searchBuffer[256] = "";
    std::string m_currentDirectory = "src/objects";
    std::string m_selectedFilePath;

    void drawToolbar();
    void drawFolderTree();
    void drawFileView();
    void registerObject();
    void registerTexture();
    void queueAddObjectEvent(const std::string& metaPath, const std::string& filename, const std::string& stem);
    void queueAssignTextureEvent(const std::string& metaPath, const std::string& filename, const std::string& stem);
};
