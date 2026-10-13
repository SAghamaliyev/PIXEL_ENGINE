#pragma once

#include <imgui.h>

struct PanelRect {
    ImVec2 pos = ImVec2(0.0f, 0.0f);
    ImVec2 size = ImVec2(0.0f, 0.0f);
};

struct EditorLayout {
    PanelRect hierarchy;
    PanelRect inspector;
    PanelRect contentBrowser;
    PanelRect console;
    PanelRect viewport;

    int viewportX = 0;
    int viewportY = 0;
    int viewportW = 0;
    int viewportH = 0;
};

constexpr ImGuiWindowFlags EDITOR_PANEL_WINDOW_FLAGS =
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoBringToFrontOnFocus |
    ImGuiWindowFlags_NoTitleBar;

inline EditorLayout computeEditorLayout(int screenWidth, int screenHeight, float menuBarHeight) {
    EditorLayout layout;

    const float total_h = static_cast<float>(screenHeight) - menuBarHeight;
    const float left_w = static_cast<float>(screenWidth) * 0.18f;
    const float right_w = static_cast<float>(screenWidth) * 0.22f;
    const float bottom_h = total_h * 0.30f;
    const float viewport_w = static_cast<float>(screenWidth) - left_w - right_w;
    const float viewport_h = total_h - bottom_h;

    // Total bottom width spanning under the Hierarchy and Viewport up to the Inspector
    const float bottom_total_w = static_cast<float>(screenWidth) - right_w;
    const float browser_w = bottom_total_w * 0.46f;
    const float console_w = bottom_total_w - browser_w;

    layout.hierarchy.pos = ImVec2(0.0f, menuBarHeight);
    layout.hierarchy.size = ImVec2(left_w, viewport_h);

    layout.inspector.pos = ImVec2(static_cast<float>(screenWidth) - right_w, menuBarHeight);
    layout.inspector.size = ImVec2(right_w, total_h);

    layout.contentBrowser.pos = ImVec2(0.0f, menuBarHeight + viewport_h);
    layout.contentBrowser.size = ImVec2(browser_w, bottom_h);

    layout.console.pos = ImVec2(browser_w, menuBarHeight + viewport_h);
    layout.console.size = ImVec2(console_w, bottom_h);

    layout.viewport.pos = ImVec2(left_w, menuBarHeight);
    layout.viewport.size = ImVec2(viewport_w, viewport_h);

    layout.viewportX = static_cast<int>(left_w);
    layout.viewportY = static_cast<int>(bottom_h);
    layout.viewportW = static_cast<int>(viewport_w);
    layout.viewportH = static_cast<int>(viewport_h);

    return layout;
}

inline void applyPanelRect(const PanelRect& panelRect) {
    ImGui::SetNextWindowPos(panelRect.pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(panelRect.size, ImGuiCond_Always);
}
