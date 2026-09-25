#include "UISystem.h"
#include "Theme/EditorTheme.h"
#include "../Core/Logger/Logger.h"
#include "../Core/EventSystem/EventSystem.h"

#include "Materials/imgui.h"
#include "Materials/Backends/imgui_impl_glfw.h"
#include "Materials/Backends/imgui_impl_opengl3.h"
#include "../dependencies/ImGuizmo.h"

#include <GLFW/glfw3.h>

void UISystem::init(GLFWwindow* window) {
    m_window = window;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontDefault();
    io.FontGlobalScale = 1.15f;

    EditorTheme::setDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_hierarchy.setHierarchyViews(&m_hierarchyViews);
    m_inspector.setContentBrowserPanel(&m_contentBrowser);

    Logger::addLog(LOG_INFO, "UI initialized");
}

void UISystem::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void UISystem::render() {
    updateLayout();
    m_menuBar.draw(m_window, m_windowShouldClose, m_hierarchy, m_inspector, m_contentBrowser, m_console);
    drawPanels();
}

void UISystem::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UISystem::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

long UISystem::getSelectedEntityID() const {
    return m_hasSelectedEntity ? m_selectedEntity.entityID : -1;
}

void UISystem::setHierarchyViews(const std::vector<EditorHierarchyView>& hierarchyViews) {
    m_hierarchyViews = hierarchyViews;
    m_hierarchy.setHierarchyViews(&m_hierarchyViews);
}

void UISystem::setSelectedEntityView(const EditorEntityView& entityView) {
    m_selectedEntity = entityView;
    m_hasSelectedEntity = true;
    m_hierarchy.setSelectedEntityID(entityView.entityID);
}

void UISystem::clearSelectedEntityView() {
    m_hasSelectedEntity = false;
    m_hierarchy.setSelectedEntityID(-1);
}

void UISystem::setGizmoOperation(EditorGizmoOperation operation) {
    m_gizmo.setOperation(operation);
}

EditorGizmoOperation UISystem::getGizmoOperation() const {
    return m_gizmo.getOperation();
}

void UISystem::getViewportRect(int& outX, int& outY, int& outW, int& outH) {
    if (!m_window) {
        return;
    }

    int screenW = 0;
    int screenH = 0;
    glfwGetFramebufferSize(m_window, &screenW, &screenH);

    const EditorLayout layout = computeEditorLayout(screenW, screenH, ImGui::GetFrameHeight());
    outX = layout.viewportX;
    outY = layout.viewportY;
    outW = layout.viewportW;
    outH = layout.viewportH;
}

void UISystem::updateLayout() {
    int screenW = 0;
    int screenH = 0;
    glfwGetFramebufferSize(m_window, &screenW, &screenH);
    m_layout = computeEditorLayout(screenW, screenH, ImGui::GetFrameHeight());
}

void UISystem::drawPanels() {
    m_hierarchy.draw(m_layout);

    const EditorEntityView* selected = findSelectedEntityView();
    m_inspector.setSelectedEntity(selected);
    m_inspector.setGizmoOperation(m_gizmo.getOperation());
    m_inspector.draw(m_layout);

    m_contentBrowser.draw(m_layout);
    m_console.draw(m_layout);

    m_viewport.draw(m_layout, m_gizmo.getOperation(), selected);

    if (!m_window) {
        return;
    }

    int screenW = 0;
    int screenH = 0;
    glfwGetFramebufferSize(m_window, &screenW, &screenH);
    m_gizmo.draw(selected, m_layout, screenH);
}

const EditorEntityView* UISystem::findSelectedEntityView() const {
    return m_hasSelectedEntity ? &m_selectedEntity : nullptr;
}
