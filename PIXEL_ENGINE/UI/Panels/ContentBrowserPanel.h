#pragma once

#include "../EditorLayout.h"
#include "../../EventSystem/EventSystem.h"

#include <string>
#include <vector>

class ConsolePanel;

class ContentBrowserPanel {
public:
    void setEventQueue(EventSystem* events);
    void setConsole(ConsolePanel* console);

    void draw(const EditorLayout& layout);

    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

    void setTextureSelectionMode(bool enabled, unsigned int entityID = 0);

private:
    EventSystem* m_events = nullptr;
    ConsolePanel* m_console = nullptr;
    bool m_visible = true;

    bool m_textureSelectionMode = false;
    unsigned int m_selectedEntityIDForTexture = 0;

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
    void pushEvent(const EditorEvent& event);
};
