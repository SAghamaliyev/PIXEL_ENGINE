#pragma once

#include "Materials/imgui.h"

struct PanelRect {
    ImVec2 pos;
    ImVec2 size;
};

struct EditorLayout {
    PanelRect hierarchy;
    PanelRect inspector;
    PanelRect contentBrowser;
    PanelRect console;

    int viewportX = 0;
    int viewportY = 0;
    int viewportW = 0;
    int viewportH = 0;
};

inline constexpr ImGuiWindowFlags kEditorPanelWindowFlags =
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoBringToFrontOnFocus;

inline EditorLayout computeEditorLayout(int screenW, int screenH, float menuBarH) {
    EditorLayout layout;

    const float totalH = (float)screenH - menuBarH;
    const float leftW = (float)screenW * 0.18f;
    const float rightW = (float)screenW * 0.22f;
    const float topH = totalH * 0.58f;
    const float bottomH = totalH * 0.42f;
    const float halfBottomW = ((float)screenW - rightW) * 0.55f;

    layout.hierarchy.pos = ImVec2(0.0f, menuBarH);
    layout.hierarchy.size = ImVec2(leftW, topH);

    layout.inspector.pos = ImVec2((float)screenW - rightW, menuBarH);
    layout.inspector.size = ImVec2(rightW, totalH);

    layout.contentBrowser.pos = ImVec2(0.0f, menuBarH + topH);
    layout.contentBrowser.size = ImVec2(halfBottomW, bottomH);

    layout.console.pos = ImVec2(halfBottomW, menuBarH + topH);
    layout.console.size = ImVec2((float)screenW - halfBottomW, bottomH);

    layout.viewportX = (int)leftW;
    layout.viewportY = (int)bottomH;
    layout.viewportW = (int)((float)screenW - leftW - rightW);
    layout.viewportH = (int)(totalH * 0.62f);

    return layout;
}

inline void applyPanelRect(const PanelRect& rect) {
    ImGui::SetNextWindowPos(rect.pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(rect.size, ImGuiCond_Always);
}
