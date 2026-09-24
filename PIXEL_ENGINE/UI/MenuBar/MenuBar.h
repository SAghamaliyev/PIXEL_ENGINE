#pragma once

struct GLFWwindow;

class SceneHierarchyPanel;
class InspectorPanel;
class ContentBrowserPanel;
class ConsolePanel;

class MenuBar {
public:
    void draw(GLFWwindow* window, bool& windowShouldClose,
        SceneHierarchyPanel& hierarchy,
        InspectorPanel& inspector,
        ContentBrowserPanel& contentBrowser,
        ConsolePanel& console);
};
