#include "UISystem.h"

#include <vector>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include "Core/EventSystem/EventSystem.h"
#include "Core/Logger/Logger.h"
#include "Systems/UI/Theme/EditorTheme.h"

void UISystem::init(GLFWwindow* pWindow) {
    m_pWindow = pWindow;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontDefault();
    io.FontGlobalScale = 1.05f;

    EditorTheme::setDark();

    ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_hierarchy.setHierarchyViews(&m_hierarchyViews);
    m_inspector.setContentBrowserPanel(&m_contentBrowser);

    Logger::addLog(LogType::LOG_INFO, "UI initialized");
}

void UISystem::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void UISystem::render() {
    updateLayout();
    m_menuBar.draw(m_pWindow, m_windowShouldClose, m_hierarchy, m_inspector, m_contentBrowser, m_console);
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

void UISystem::setGizmoOperation(GizmoOperation operation) {
    m_gizmo.setOperation(operation);
}

GizmoOperation UISystem::getGizmoOperation() const {
    return m_gizmo.getOperation();
}

void UISystem::getViewportRect(int& outX, int& outY, int& outW, int& outH) {
    if (m_pWindow == nullptr) {
        return;
    }

    int screen_w = 0;
    int screen_h = 0;
    glfwGetFramebufferSize(m_pWindow, &screen_w, &screen_h);

    const EditorLayout layout = computeEditorLayout(screen_w, screen_h, ImGui::GetFrameHeight());
    outX = layout.viewportX;
    outY = layout.viewportY;
    outW = layout.viewportW;
    outH = layout.viewportH;
}

void UISystem::updateLayout() {
    int screen_w = 0;
    int screen_h = 0;
    glfwGetFramebufferSize(m_pWindow, &screen_w, &screen_h);
    m_layout = computeEditorLayout(screen_w, screen_h, ImGui::GetFrameHeight());
}

void UISystem::drawPanels() {
    m_hierarchy.draw(m_layout);

    const EditorEntityView* p_selected = findSelectedEntityView();
    m_inspector.setSelectedEntity(p_selected);
    m_inspector.setGizmoOperation(m_gizmo.getOperation());
    m_inspector.draw(m_layout);

    m_contentBrowser.draw(m_layout);
    m_console.draw(m_layout);

    m_viewport.draw(m_layout, m_gizmo.getOperation(), p_selected);

    if (m_pWindow == nullptr) {
        return;
    }

    int screen_w = 0;
    int screen_h = 0;
    glfwGetFramebufferSize(m_pWindow, &screen_w, &screen_h);
    m_gizmo.draw(p_selected, m_layout, screen_h);
}

const EditorEntityView* UISystem::findSelectedEntityView() const {
    return m_hasSelectedEntity ? &m_selectedEntity : nullptr;
}
