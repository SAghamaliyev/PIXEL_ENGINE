#pragma once

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ConsolePanel.h"
#include "EditorLayout.h"
#include "../EventSystem/EventSystem.h"

#include <string>
#include <vector>

struct GLFWwindow;  // forward declaration

class EditorUI {
private:
    GLFWwindow* m_window = nullptr;
    bool m_windowShouldClose = false;
    EditorLayout m_layout;
    EventSystem m_eventSystem;
    std::vector<EditorEntityView> m_entityViews;

    SceneHierarchyPanel m_hierarchy;
    InspectorPanel m_inspector;
    ContentBrowserPanel m_contentBrowser;
    ConsolePanel m_console;

    void drawMainMenuBar();
    void drawPanels();
    void drawPerformanceOverlay();
    void updateLayout();

public:
    EditorUI() = default;
    ~EditorUI() = default;

    void init(GLFWwindow* window, void* engineEventTarget = nullptr);
    void beginFrame();
    void render();
    void endFrame();
    void shutdown();

    bool WindowShouldClose() const { return m_windowShouldClose; }

    int getSelectedEntityID() const;
    void getViewportRect(int& outX, int& outY, int& outW, int& outH);
    void setEntityViews(const std::vector<EditorEntityView>& entityViews);

    // /FLAG Engine can read queued UI flags without the UI knowing engine modules.
    const std::vector<EditorEvent>& getEvents() const { return m_eventSystem.getEvents(); }

    // /FLAG Engine can consume UI flags after it handles them.
    std::vector<EditorEvent> consumeEvents();

    // /FLAG Engine can clear UI flags when it has processed or discarded them.
    void clearEvents() { m_eventSystem.clear(); }
};
