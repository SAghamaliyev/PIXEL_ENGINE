#pragma once

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ConsolePanel.h"
#include "EditorLayout.h"
#include "../Core/EventSystem/EventSystem.h"

#include <string>
#include <vector>

struct GLFWwindow;  // forward declaration
class SceneSystem;

class EditorUI {
private:
    GLFWwindow* m_window = nullptr;
    SceneSystem* m_sceneSystem = nullptr;
    bool m_windowShouldClose = false;
    EditorLayout m_layout;
    std::vector<EditorEntityView> m_entityViews;
    EditorGizmoOperation m_gizmoOperation = EditorGizmoOperation::Translate;
    int m_activeRotateAxis = -1;
    float m_rotateDragStartAngle = 0.0f;
    float m_rotateDragStart[3] = { 0.0f, 0.0f, 0.0f };

    SceneHierarchyPanel m_hierarchy;
    InspectorPanel m_inspector;
    ContentBrowserPanel m_contentBrowser;
    ConsolePanel m_console;

    void drawMainMenuBar();
    void drawPanels();
    void drawGizmo();
    void drawRotateGizmo(const Transform& transform, unsigned int entityID,
        float viewportX, float viewportY, float viewportW, float viewportH);
    void drawPerformanceOverlay();
    void updateLayout();
    const EditorEntityView* findSelectedEntityView() const;

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
    void setGizmoOperation(EditorGizmoOperation operation) { m_gizmoOperation = operation; }
    EditorGizmoOperation getGizmoOperation() const { return m_gizmoOperation; }

};
