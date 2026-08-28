#include "EditorTheme.h"
#include "Materials/imgui.h"

namespace EditorTheme {

void setDark() {
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.FrameRounding = 2.0f;
    style.ScrollbarRounding = 2.0f;
    style.WindowBorderSize = 1.0f;
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(8, 4);
}

void setBlue() {
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.1f, 0.2f, 1.0f);
    colors[ImGuiCol_Header] = ImVec4(0.15f, 0.3f, 0.5f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.2f, 0.4f, 0.6f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.45f, 0.65f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.15f, 0.3f, 0.5f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.2f, 0.4f, 0.6f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 0.45f, 0.65f, 1.0f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.1f, 0.15f, 0.25f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.25f, 0.35f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.2f, 0.3f, 0.4f, 1.0f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.12f, 0.2f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.18f, 0.28f, 1.0f);
}

void setPurple() {
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.08f, 0.15f, 1.0f);
    colors[ImGuiCol_Header] = ImVec4(0.4f, 0.2f, 0.5f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.5f, 0.25f, 0.6f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.6f, 0.3f, 0.7f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.4f, 0.2f, 0.5f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.25f, 0.6f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.6f, 0.3f, 0.7f, 1.0f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.1f, 0.25f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.15f, 0.35f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.4f, 0.2f, 0.45f, 1.0f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.05f, 0.12f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.25f, 0.1f, 0.3f, 1.0f);
}

}
