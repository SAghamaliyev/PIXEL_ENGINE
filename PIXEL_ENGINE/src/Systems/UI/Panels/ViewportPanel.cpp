#include "ViewportPanel.h"

#include <string>

#include <imgui.h>

#include "Core/EventSystem/EventSystem.h"
#include "Systems/UI/Theme/EditorTheme.h"

void ViewportPanel::draw(const EditorLayout& layout, GizmoOperation operation, const EditorEntityView* pSelectedEntity) {
    drawChrome(layout);
    drawToolbar(layout, operation);
    drawPerformanceOverlay(layout);
    drawStatusBar(layout, pSelectedEntity);
}

void ViewportPanel::drawChrome(const EditorLayout& layout) {
    ImDrawList* p_draw_list = ImGui::GetBackgroundDrawList();
    const ImVec2 min_pos = layout.viewport.pos;
    const ImVec2 max_pos = ImVec2(
        layout.viewport.pos.x + layout.viewport.size.x,
        layout.viewport.pos.y + layout.viewport.size.y);

    p_draw_list->AddRectFilled(
        min_pos,
        ImVec2(max_pos.x, min_pos.y + 28.0f),
        IM_COL32(12, 14, 18, 210));
    p_draw_list->AddRect(min_pos, max_pos, IM_COL32(42, 48, 58, 255), 0.0f, 0, 1.5f);

    ImGui::SetNextWindowPos(ImVec2(min_pos.x + 10.0f, min_pos.y + 5.0f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);
    constexpr ImGuiWindowFlags TITLE_FLAGS =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoInputs;
    if (ImGui::Begin("##ViewportTitle", nullptr, TITLE_FLAGS)) {
        ImGui::PushStyleColor(ImGuiCol_Text, EditorTheme::accent());
        ImGui::TextUnformatted("SCENE");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::TextDisabled("Viewport");
    }
    ImGui::End();
}

void ViewportPanel::drawToolbar(const EditorLayout& layout, GizmoOperation operation) {
    ImGui::SetNextWindowPos(
        ImVec2(layout.viewport.pos.x + 12.0f, layout.viewport.pos.y + 36.0f),
        ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.90f);

    constexpr ImGuiWindowFlags FLAGS =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoNav;

    if (!ImGui::Begin("##ViewportToolbar", nullptr, FLAGS)) {
        ImGui::End();
        return;
    }

    const char* labels[] = { "Move", "Rotate", "Scale" };
    const GizmoOperation operations[] = {
        GizmoOperation::Translate,
        GizmoOperation::Rotate,
        GizmoOperation::Scale
    };

    for (int i = 0; i < 3; ++i) {
        const bool is_selected = (operation == operations[i]);
        if (is_selected) {
            ImGui::PushStyleColor(ImGuiCol_Button, EditorTheme::accent());
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, EditorTheme::accentHover());
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        }

        if (ImGui::Button(labels[i], ImVec2(72.0f, 0.0f))) {
            EditorEvent gizmo_event;
            if (i == 0) {
                gizmo_event.type = EditorEventType::SetGizmoTranslate;
            }
            else if (i == 1) {
                gizmo_event.type = EditorEventType::SetGizmoRotate;
            }
            else if (i == 2) {
                gizmo_event.type = EditorEventType::SetGizmoScale;
            }
            EventSystem::pushEvent(gizmo_event);
        }

        if (is_selected) {
            ImGui::PopStyleColor(3);
        }

        if (i < 2) {
            ImGui::SameLine();
        }
    }

    ImGui::End();
}

void ViewportPanel::drawPerformanceOverlay(const EditorLayout& layout) {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowPos(
        ImVec2(
            layout.viewport.pos.x + layout.viewport.size.x - 12.0f,
            layout.viewport.pos.y + 36.0f),
        ImGuiCond_Always,
        ImVec2(1.0f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.90f);

    constexpr ImGuiWindowFlags FLAGS =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav;

    if (ImGui::Begin("##Performance", nullptr, FLAGS)) {
        ImGui::Text("FPS  %.0f", io.Framerate);
        ImGui::TextDisabled("%.2f ms", 1000.0f / io.Framerate);
    }
    ImGui::End();
}

void ViewportPanel::drawStatusBar(const EditorLayout& layout, const EditorEntityView* pSelectedEntity) {
    const float bar_height = 26.0f;
    const ImVec2 pos = ImVec2(
        layout.viewport.pos.x + 8.0f,
        layout.viewport.pos.y + layout.viewport.size.y - bar_height - 8.0f);

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(layout.viewport.size.x - 16.0f, bar_height), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.88f);

    constexpr ImGuiWindowFlags FLAGS =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoInputs;

    if (!ImGui::Begin("##ViewportStatus", nullptr, FLAGS)) {
        ImGui::End();
        return;
    }

    if (pSelectedEntity != nullptr) {
        ImGui::Text("%s  ·  ID %ld  ·  Mesh %llu",
            pSelectedEntity->name.c_str(),
            pSelectedEntity->entityID,
            static_cast<unsigned long long>(pSelectedEntity->meshID));
    }
    else {
        ImGui::TextDisabled("Nothing selected");
    }

    ImGui::End();
}
