#pragma once

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ConsolePanel.h"
#include "EditorLayout.h"
#include "../SceneSystem/SceneSystem.h"

#include <string>

struct GLFWwindow;  // forward declaration

class EditorUI {
private:
    GLFWwindow* m_window = nullptr;
    SceneSystem* m_sceneSystem = nullptr;
    bool m_windowShouldClose = false;
    EditorLayout m_layout;

    SceneHierarchyPanel m_hierarchy;
    InspectorPanel m_inspector;
    ContentBrowserPanel m_contentBrowser;
    ConsolePanel m_console;

    void drawMainMenuBar();
    void drawPanels();
    void updateLayout();

public:
    EditorUI() = default;
    ~EditorUI() = default;

    void init(GLFWwindow* window, SceneSystem* sceneSystem);
    void beginFrame();
    void render();
    void endFrame();
    void shutdown();

    bool WindowShouldClose() const { return m_windowShouldClose; }

    int getSelectedEntityID() const;
    void getViewportRect(int& outX, int& outY, int& outW, int& outH);

    void addLog(ConsolePanel::LogEntry::Type type, const std::string& message);
    void clearLogs();
};
