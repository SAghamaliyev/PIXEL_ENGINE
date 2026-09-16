#include "EditorUI.h"
#include "EditorTheme.h"
#include "../Logger/Logger.h"

#include "Materials/imgui.h"
#include "Materials/Backends/imgui_impl_glfw.h"
#include "Materials/Backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

void EditorUI::init(GLFWwindow* window, void* engineEventTarget) {
    m_window = window;
    (void)engineEventTarget;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontDefault();
    io.FontGlobalScale = 1.3f;

    EditorTheme::setDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_hierarchy.setEntityViews(&m_entityViews);
    m_inspector.setEntityViews(&m_entityViews);
    m_inspector.setContentBrowserPanel(&m_contentBrowser);
    m_contentBrowser.setConsole(&m_console);

    Logger::addLog(LOG_INFO, "Engine initialized");
}

void EditorUI::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void EditorUI::render() {
    updateLayout();
    drawMainMenuBar();
    drawPanels();
}

void EditorUI::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorUI::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int EditorUI::getSelectedEntityID() const {
    return m_hierarchy.getSelectedEntityID();
}

void EditorUI::setEntityViews(const std::vector<EditorEntityView>& entityViews) {
    m_entityViews = entityViews;
    m_hierarchy.setEntityViews(&m_entityViews);
    m_inspector.setEntityViews(&m_entityViews);
}

std::vector<EditorEvent> EditorUI::consumeEvents() {
    std::vector<EditorEvent> events = EventSystem::getEvents();
    EventSystem::clear();
    return events;
}

void EditorUI::getViewportRect(int& outX, int& outY, int& outW, int& outH) {
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

void EditorUI::updateLayout() {
    int screenW = 0;
    int screenH = 0;
    glfwGetFramebufferSize(m_window, &screenW, &screenH);
    m_layout = computeEditorLayout(screenW, screenH, ImGui::GetFrameHeight());
}

void EditorUI::drawMainMenuBar() {
    if (!ImGui::BeginMainMenuBar()) {
        return;
    }

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
            // /FLAG ClearScene: UI asks the engine to clear the active scene.
            EventSystem::push(EditorEvent{ EditorEventType::ClearScene });
        }
        if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
            // /FLAG OpenScene: UI asks the engine to open a scene picker/load flow.
            EventSystem::push(EditorEvent{ EditorEventType::OpenScene });
        }
        if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
            // /FLAG SaveScene: UI asks the engine to save the current scene.
            EventSystem::push(EditorEvent{ EditorEventType::SaveScene });
        }
        if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) {
            // /FLAG SaveSceneAs: UI asks the engine to save the scene with a new path.
            EventSystem::push(EditorEvent{ EditorEventType::SaveSceneAs });
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit", "Alt+F4")) {
            m_windowShouldClose = true;
            if (m_window) {
                glfwSetWindowShouldClose(m_window, GLFW_TRUE);
            }
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
            // TODO: undo the last editor action.
        }
        if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
            // TODO: redo the last undone action.
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Preferences")) {
            // TODO: open editor preferences.
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
        bool showHierarchy = m_hierarchy.isVisible();
        if (ImGui::MenuItem("Scene Hierarchy", nullptr, &showHierarchy)) {
            m_hierarchy.setVisible(showHierarchy);
        }

        bool showInspector = m_inspector.isVisible();
        if (ImGui::MenuItem("Inspector", nullptr, &showInspector)) {
            m_inspector.setVisible(showInspector);
        }

        bool showContentBrowser = m_contentBrowser.isVisible();
        if (ImGui::MenuItem("Content Browser", nullptr, &showContentBrowser)) {
            m_contentBrowser.setVisible(showContentBrowser);
        }

        bool showConsole = m_console.isVisible();
        if (ImGui::MenuItem("Console", nullptr, &showConsole)) {
            m_console.setVisible(showConsole);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Theme")) {
        if (ImGui::MenuItem("Dark Theme")) {
            EditorTheme::setDark();
        }
        if (ImGui::MenuItem("Blue Theme")) {
            EditorTheme::setBlue();
        }
        if (ImGui::MenuItem("Purple/Pink Theme")) {
            EditorTheme::setPurple();
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help")) {
        if (ImGui::MenuItem("About")) {
            // /FLAG ShowAbout: UI asks the engine/app layer to show or log the about message.
            EditorEvent event;
            event.type = EditorEventType::ShowAbout;
            event.message = "Hello, this is Pixel Engine, i am glad you downloaded it.\n"
                "If you want to add something or noticed some problems\n"
                "please dont be hesitated to contact me: saidaghamaliyev@gmail.com\n"
                "Please ENJOY, Said Aghamaliyev";
            EventSystem::push(event);
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void EditorUI::drawPanels() {
    m_hierarchy.draw(m_layout);

    m_inspector.setSelectedEntityID(m_hierarchy.getSelectedEntityID());
    m_inspector.draw(m_layout);

    m_contentBrowser.draw(m_layout);
    m_console.draw(m_layout);
    drawPerformanceOverlay();
}

void EditorUI::drawPerformanceOverlay() {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowPos(
        ImVec2(io.DisplaySize.x - 10.0f, ImGui::GetFrameHeight() + 10.0f),
        ImGuiCond_Always,
        ImVec2(1.0f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.7f);

    constexpr ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav;

    if (ImGui::Begin("Performance", nullptr, flags)) {
        ImGui::Text("FPS: %.1f", io.Framerate);
        ImGui::Text("Frame time: %.2f ms", 1000.0f / io.Framerate);
    }
    ImGui::End();
}
