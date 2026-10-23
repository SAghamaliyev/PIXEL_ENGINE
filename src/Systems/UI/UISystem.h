#pragma once

#include <vector>

#include "Core/Definitions/CommonDefinitions.h"
#include "Systems/UI/EditorViews.h"
#include "Systems/UI/Gizmo/Gizmo.h"
#include "Systems/UI/Layout/EditorLayout.h"
#include "Systems/UI/MenuBar/MenuBar.h"
#include "Systems/UI/Panels/ConsolePanel.h"
#include "Systems/UI/Panels/ContentBrowserPanel.h"
#include "Systems/UI/Panels/InspectorPanel.h"
#include "Systems/UI/Panels/SceneHierarchyPanel.h"
#include "Systems/UI/Panels/ViewportPanel.h"

struct GLFWwindow;

class UISystem {
private:
    GLFWwindow* m_pWindow = nullptr;
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

    void init(GLFWwindow* pWindow);
    void beginFrame();
    void render();
    void endFrame();
    void shutdown();

    bool windowShouldClose() const {
        return m_windowShouldClose;
    }

    long getSelectedEntityID() const;
    void getViewportRect(int& outX, int& outY, int& outW, int& outH);
    void setHierarchyViews(const std::vector<EditorHierarchyView>& hierarchyViews);
    void setSelectedEntityView(const EditorEntityView& entityView);
    void clearSelectedEntityView();
    void setGizmoOperation(GizmoOperation operation);
    GizmoOperation getGizmoOperation() const;
};
