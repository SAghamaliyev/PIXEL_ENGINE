#include "MenuBar.h"

#include <GLFW/glfw3.h>
#include <imgui.h>

#include "Core/EventSystem/EventSystem.h"
#include "Systems/UI/Panels/ConsolePanel.h"
#include "Systems/UI/Panels/ContentBrowserPanel.h"
#include "Systems/UI/Panels/InspectorPanel.h"
#include "Systems/UI/Panels/SceneHierarchyPanel.h"
#include "Systems/UI/Theme/EditorTheme.h"

void MenuBar::draw(GLFWwindow* pWindow, bool& windowShouldClose,
    SceneHierarchyPanel& hierarchy,
    InspectorPanel& inspector,
    ContentBrowserPanel& contentBrowser,
    ConsolePanel& console) {
    if (!ImGui::BeginMainMenuBar()) {
        return;
    }

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
            EditorEvent event;
            event.type = EditorEventType::ClearScene;
            EventSystem::pushEvent(event);
        }
        if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
            EditorEvent event;
            event.type = EditorEventType::OpenScene;
            EventSystem::pushEvent(event);
        }
        if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
            EditorEvent event;
            event.type = EditorEventType::SaveScene;
            EventSystem::pushEvent(event);
        }
        if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) {
            EditorEvent event;
            event.type = EditorEventType::SaveSceneAs;
            EventSystem::pushEvent(event);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit", "Alt+F4")) {
            windowShouldClose = true;
            if (pWindow != nullptr) {
                glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
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
        bool show_hierarchy = hierarchy.isVisible();
        if (ImGui::MenuItem("Scene Hierarchy", nullptr, &show_hierarchy)) {
            hierarchy.setVisible(show_hierarchy);
        }

        bool show_inspector = inspector.isVisible();
        if (ImGui::MenuItem("Inspector", nullptr, &show_inspector)) {
            inspector.setVisible(show_inspector);
        }

        bool show_content_browser = contentBrowser.isVisible();
        if (ImGui::MenuItem("Content Browser", nullptr, &show_content_browser)) {
            contentBrowser.setVisible(show_content_browser);
        }

        bool show_console = console.isVisible();
        if (ImGui::MenuItem("Terminal / Console", nullptr, &show_console)) {
            console.setVisible(show_console);
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
