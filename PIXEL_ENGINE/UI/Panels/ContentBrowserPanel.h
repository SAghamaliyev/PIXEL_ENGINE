#pragma once

#include "../EditorLayout.h"
#include "../EditorEvents.h"

#include <string>
#include <vector>

class ConsolePanel;

class ContentBrowserPanel {
public:
    void setEventQueue(std::vector<EditorEvent>* events);
    void setConsole(ConsolePanel* console);

    void draw(const EditorLayout& layout);

    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

private:
    std::vector<EditorEvent>* m_events = nullptr;
    ConsolePanel* m_console = nullptr;
    bool m_visible = true;

    char m_searchBuffer[256] = "";
    std::string m_currentDirectory = "src/objects";
    std::string m_selectedFilePath;

    void drawToolbar();
    void drawFolderTree();
    void drawFileView();
    void registerObject();
    void queueAddObjectEvent(const std::string& metaPath, const std::string& filename, const std::string& stem);
    void pushEvent(const EditorEvent& event);
};
