#pragma once

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ConsolePanel.h"
#include "Panels/ViewportPanel.h"
#include "Gizmo/Gizmo.h"
#include "MenuBar/MenuBar.h"
#include "Layout/EditorLayout.h"

#include <vector>

struct GLFWwindow;

class UISystem {
private:
    GLFWwindow* m_window = nullptr;
    bool m_windowShouldClose = false;
    EditorLayout m_layout;
    std::vector<EditorHierarchyView> m_hierarchyViews;
    EditorEntityView m_selectedEntity;
    bool m_hasSelectedEntity = false;

    SceneHierarchyPanel m_hierarchy;
    InspectorPanel m_inspector;
    ContentBrowserPanel m_contentBrowser;
    ConsolePanel m_console;
    ViewportPanel m_viewport;
    Gizmo m_gizmo;
    MenuBar m_menuBar;

    void updateLayout();
    void drawPanels();
    const EditorEntityView* findSelectedEntityView() const;

public:
    UISystem() = default;
    ~UISystem() = default;

    void init(GLFWwindow* window);
    void beginFrame();
    void render();
    void endFrame();
    void shutdown();

    bool WindowShouldClose() const { return m_windowShouldClose; }

    long getSelectedEntityID() const;
    void getViewportRect(int& outX, int& outY, int& outW, int& outH);
    void setHierarchyViews(const std::vector<EditorHierarchyView>& hierarchyViews);
    void setSelectedEntityView(const EditorEntityView& entityView);
    void clearSelectedEntityView();
    void setGizmoOperation(EditorGizmoOperation operation);
    EditorGizmoOperation getGizmoOperation() const;
};
