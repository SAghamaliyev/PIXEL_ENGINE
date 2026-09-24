#pragma once

#include "../Materials/imgui.h"

struct PanelRect {
    ImVec2 pos;
    ImVec2 size;
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

inline constexpr ImGuiWindowFlags kEditorPanelWindowFlags =
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoBringToFrontOnFocus |
    ImGuiWindowFlags_NoTitleBar;

inline EditorLayout computeEditorLayout(int screenW, int screenH, float menuBarH) {
    EditorLayout layout;

    const float totalH = (float)screenH - menuBarH;
    const float leftW = (float)screenW * 0.20f;
    const float rightW = (float)screenW * 0.24f;
    const float bottomH = totalH * 0.32f;
    const float viewportW = (float)screenW - leftW - rightW;
    const float viewportH = totalH - bottomH;
    const float browserW = viewportW * 0.58f;

    layout.hierarchy.pos = ImVec2(0.0f, menuBarH);
    layout.hierarchy.size = ImVec2(leftW, viewportH);

    layout.inspector.pos = ImVec2((float)screenW - rightW, menuBarH);
    layout.inspector.size = ImVec2(rightW, totalH);

    layout.contentBrowser.pos = ImVec2(0.0f, menuBarH + viewportH);
    layout.contentBrowser.size = ImVec2(leftW + browserW, bottomH);

    layout.console.pos = ImVec2(leftW + browserW, menuBarH + viewportH);
    layout.console.size = ImVec2((float)screenW - leftW - browserW, bottomH);

    layout.viewport.pos = ImVec2(leftW, menuBarH);
    layout.viewport.size = ImVec2(viewportW, viewportH);

    layout.viewportX = (int)leftW;
    layout.viewportY = (int)bottomH;
    layout.viewportW = (int)viewportW;
    layout.viewportH = (int)viewportH;

    return layout;
}

inline void applyPanelRect(const PanelRect& rect) {
    ImGui::SetNextWindowPos(rect.pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(rect.size, ImGuiCond_Always);
}
