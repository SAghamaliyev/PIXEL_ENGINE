#include "MenuBar.h"

#include "../Panels/SceneHierarchyPanel.h"
#include "../Panels/InspectorPanel.h"
#include "../Panels/ContentBrowserPanel.h"
#include "../Panels/ConsolePanel.h"
#include "../Theme/EditorTheme.h"
#include "../Materials/imgui.h"
#include "../../Core/EventSystem/EventSystem.h"

#include <GLFW/glfw3.h>

void MenuBar::draw(GLFWwindow* window, bool& windowShouldClose,
    SceneHierarchyPanel& hierarchy,
    InspectorPanel& inspector,
    ContentBrowserPanel& contentBrowser,
    ConsolePanel& console) {
    if (!ImGui::BeginMainMenuBar()) {
        return;
    }

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
            EventSystem::pushEvent(EditorEvent{ EditorEventType::ClearScene });
        }
        if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
            EventSystem::pushEvent(EditorEvent{ EditorEventType::OpenScene });
        }
        if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
            EventSystem::pushEvent(EditorEvent{ EditorEventType::SaveScene });
        }
        if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) {
            EventSystem::pushEvent(EditorEvent{ EditorEventType::SaveSceneAs });
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit", "Alt+F4")) {
            windowShouldClose = true;
            if (window) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit")) {
        ImGui::MenuItem("Undo", "Ctrl+Z", false, false);
        ImGui::MenuItem("Redo", "Ctrl+Y", false, false);
        ImGui::Separator();
        ImGui::MenuItem("Preferences", nullptr, false, false);
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
        bool showHierarchy = hierarchy.isVisible();
        if (ImGui::MenuItem("Scene Hierarchy", nullptr, &showHierarchy)) {
            hierarchy.setVisible(showHierarchy);
        }

        bool showInspector = inspector.isVisible();
        if (ImGui::MenuItem("Inspector", nullptr, &showInspector)) {
            inspector.setVisible(showInspector);
        }

        bool showContentBrowser = contentBrowser.isVisible();
        if (ImGui::MenuItem("Content Browser", nullptr, &showContentBrowser)) {
            contentBrowser.setVisible(showContentBrowser);
        }

        bool showConsole = console.isVisible();
        if (ImGui::MenuItem("Console", nullptr, &showConsole)) {
            console.setVisible(showConsole);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Theme")) {
        if (ImGui::MenuItem("Dark")) {
            EditorTheme::setDark();
        }
        if (ImGui::MenuItem("Blue")) {
            EditorTheme::setBlue();
        }
        if (ImGui::MenuItem("Purple")) {
            EditorTheme::setPurple();
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help")) {
        if (ImGui::MenuItem("About")) {
            EditorEvent event;
            event.type = EditorEventType::ShowAbout;
            event.info.message = "Hello, this is Pixel Engine, i am glad you downloaded it.\n"
                "If you want to add something or noticed some problems\n"
                "please dont be hesitated to contact me: saidaghamaliyev@gmail.com\n"
                "Please ENJOY, Said Aghamaliyev";
            EventSystem::pushEvent(event);
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}
