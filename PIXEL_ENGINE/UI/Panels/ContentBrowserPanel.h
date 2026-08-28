#pragma once

#include "../EditorLayout.h"

#include <string>

class SceneSystem;
class ConsolePanel;

class ContentBrowserPanel {
public:
    void setSceneSystem(SceneSystem* sceneSystem);
    void setConsole(ConsolePanel* console);

    void draw(const EditorLayout& layout);

    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

private:
    SceneSystem* m_sceneSystem = nullptr;
    ConsolePanel* m_console = nullptr;
    bool m_visible = true;

    char m_searchBuffer[256] = "";
    std::string m_currentDirectory = "src";
    std::string m_selectedFilePath;

    void drawToolbar();
    void drawFolderTree();
    void drawFileView();
    void tryAddToScene(const std::string& path, const std::string& filename, const std::string& stem);
};
