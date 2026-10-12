#include "EditorTheme.h"

#include <imgui.h>

namespace {

ImVec4 g_accent(0.24f, 0.56f, 0.82f, 1.00f);
ImVec4 g_accentHover(0.30f, 0.64f, 0.90f, 1.00f);
ImVec4 g_accentActive(0.36f, 0.70f, 0.96f, 1.00f);

void applyBaseStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 8.0f;
    style.ScrollbarRounding = 10.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 6.0f;
    style.WindowBorderSize = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.WindowPadding = ImVec2(12.0f, 10.0f);
    style.FramePadding = ImVec2(8.0f, 5.0f);
    style.ItemSpacing = ImVec2(8.0f, 6.0f);
    style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
    style.IndentSpacing = 16.0f;
    style.ScrollbarSize = 12.0f;
    style.GrabMinSize = 10.0f;
    style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
}

void applyAccent(const ImVec4& accent, const ImVec4& accentHover, const ImVec4& accentActive) {
    g_accent = accent;
    g_accentHover = accentHover;
    g_accentActive = accentActive;

    ImVec4* p_colors = ImGui::GetStyle().Colors;
    p_colors[ImGuiCol_Header] = ImVec4(accent.x, accent.y, accent.z, 0.28f);
    p_colors[ImGuiCol_HeaderHovered] = ImVec4(accentHover.x, accentHover.y, accentHover.z, 0.42f);
    p_colors[ImGuiCol_HeaderActive] = ImVec4(accentActive.x, accentActive.y, accentActive.z, 0.55f);
    p_colors[ImGuiCol_Button] = ImVec4(accent.x, accent.y, accent.z, 0.72f);
    p_colors[ImGuiCol_ButtonHovered] = accentHover;
    p_colors[ImGuiCol_ButtonActive] = accentActive;
    p_colors[ImGuiCol_SliderGrab] = accentHover;
    p_colors[ImGuiCol_SliderGrabActive] = accentActive;
    p_colors[ImGuiCol_CheckMark] = accentHover;
    p_colors[ImGuiCol_SeparatorHovered] = accentHover;
    p_colors[ImGuiCol_SeparatorActive] = accentActive;
    p_colors[ImGuiCol_ResizeGrip] = ImVec4(accent.x, accent.y, accent.z, 0.35f);
    p_colors[ImGuiCol_ResizeGripHovered] = accentHover;
    p_colors[ImGuiCol_ResizeGripActive] = accentActive;
    p_colors[ImGuiCol_Tab] = ImVec4(accent.x, accent.y, accent.z, 0.45f);
    p_colors[ImGuiCol_TabHovered] = accentHover;
    p_colors[ImGuiCol_TabActive] = accentActive;
    p_colors[ImGuiCol_PlotHistogram] = accentHover;
}

}

namespace EditorTheme {

const ImVec4& accent() {
    return g_accent;
}

const ImVec4& accentHover() {
    return g_accentHover;
}

const ImVec4& accentActive() {
    return g_accentActive;
}

void setDark() {
    ImGui::StyleColorsDark();
    applyBaseStyle();

    ImVec4* p_colors = ImGui::GetStyle().Colors;
    p_colors[ImGuiCol_Text] = ImVec4(0.93f, 0.94f, 0.96f, 1.00f);
    p_colors[ImGuiCol_TextDisabled] = ImVec4(0.52f, 0.54f, 0.58f, 1.00f);
    p_colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.10f, 0.12f, 1.00f);
    p_colors[ImGuiCol_ChildBg] = ImVec4(0.11f, 0.12f, 0.15f, 1.00f);
    p_colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.12f, 0.15f, 0.98f);
    p_colors[ImGuiCol_Border] = ImVec4(0.20f, 0.22f, 0.26f, 1.00f);
    p_colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.16f, 0.20f, 1.00f);
    p_colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.21f, 0.26f, 1.00f);
    p_colors[ImGuiCol_FrameBgActive] = ImVec4(0.23f, 0.25f, 0.31f, 1.00f);
    p_colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.08f, 0.10f, 1.00f);
    p_colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.08f, 0.10f, 1.00f);
    p_colors[ImGuiCol_MenuBarBg] = ImVec4(0.07f, 0.08f, 0.10f, 1.00f);
    p_colors[ImGuiCol_ScrollbarBg] = ImVec4(0.07f, 0.08f, 0.10f, 1.00f);
    p_colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.22f, 0.26f, 1.00f);

    applyAccent(
        ImVec4(0.22f, 0.48f, 0.72f, 0.90f),
        ImVec4(0.28f, 0.60f, 0.88f, 1.00f),
        ImVec4(0.34f, 0.68f, 0.96f, 1.00f));
}

void setBlue() {
    setDark();
    applyAccent(
        ImVec4(0.16f, 0.36f, 0.64f, 0.92f),
        ImVec4(0.22f, 0.50f, 0.84f, 1.00f),
        ImVec4(0.28f, 0.58f, 0.92f, 1.00f));

    ImVec4* p_colors = ImGui::GetStyle().Colors;
    p_colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.09f, 0.14f, 1.00f);
    p_colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.12f, 0.18f, 1.00f);
    p_colors[ImGuiCol_MenuBarBg] = ImVec4(0.05f, 0.07f, 0.12f, 1.00f);
}

void setPurple() {
    setDark();
    applyAccent(
        ImVec4(0.46f, 0.24f, 0.62f, 0.92f),
        ImVec4(0.60f, 0.34f, 0.78f, 1.00f),
        ImVec4(0.70f, 0.42f, 0.88f, 1.00f));

    ImVec4* p_colors = ImGui::GetStyle().Colors;
    p_colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.07f, 0.13f, 1.00f);
    p_colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.10f, 0.18f, 1.00f);
    p_colors[ImGuiCol_MenuBarBg] = ImVec4(0.07f, 0.05f, 0.10f, 1.00f);
}

void drawPanelHeader(const char* title, const char* subtitle) {
    ImDrawList* p_draw_list = ImGui::GetWindowDrawList();
    const ImVec2 origin = ImGui::GetCursorScreenPos();
    const float height = ImGui::GetTextLineHeight() + 8.0f;
    const ImU32 accent_color = ImGui::ColorConvertFloat4ToU32(g_accentHover);

    p_draw_list->AddRectFilled(
        origin,
        ImVec2(origin.x + 3.0f, origin.y + height),
        accent_color,
        2.0f);

    ImGui::Dummy(ImVec2(8.0f, 0.0f));
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.97f, 0.99f, 1.0f));
    ImGui::TextUnformatted(title);
    ImGui::PopStyleColor();

    if (subtitle && subtitle[0] != '\0') {
        ImGui::SameLine();
        ImGui::TextDisabled("  %s", subtitle);
    }

    ImGui::Dummy(ImVec2(0.0f, 2.0f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 4.0f));
}

}
