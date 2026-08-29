#include "EditorUI.h"
#include "EditorUI.h"
#include "EditorTheme.h"
#include "../Logger/Logger.h"

#include "Materials/imgui.h"
#include "Materials/Backends/imgui_impl_glfw.h"
#include "Materials/Backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

void EditorUI::init(GLFWwindow* window, SceneSystem* sceneSystem) {
    m_window = window;
    m_sceneSystem = sceneSystem;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontDefault();
    io.FontGlobalScale = 1.3f;

    EditorTheme::setDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_hierarchy.setSceneSystem(m_sceneSystem);
    m_inspector.setSceneSystem(m_sceneSystem);
    m_contentBrowser.setSceneSystem(m_sceneSystem);
    m_contentBrowser.setConsole(&m_console);

    Logger::getInstance().addLog(Logger::LogEntry::LOG_INFO, "Engine initialized");
    // TODO: add logs through addLog() from the engine DONE!!!!
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

            m_sceneSystem->SceneClearEntityList();
            // TODO: create a new empty scene through SceneSystem. DONE!!!!
        }
        if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
            // TODO: open a scene file dialog and load it through SceneSystem.
        }
        if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
            // TODO: save the current scene through SceneSystem.
        }
        if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) {
            // TODO: save the scene with a new path through SceneSystem.
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
            Logger::getInstance().addLog(Logger::LogEntry::LOG_INFO, "Hello, this is  Pixel Engine,"
                " i am glad you downloaded it.\n If you want to add something or noticed some problems\n"
                " please dont be hesitated to contact me: saidaghamaliyev@gmail.com\n"
                " Please ENJOY, Said Aghamaliyev");
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
}
