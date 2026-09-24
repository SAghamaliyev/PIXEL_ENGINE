#include "ViewportPanel.h"

#include "../Theme/EditorTheme.h"
#include "../Materials/imgui.h"
#include "../../Core/EventSystem/EventSystem.h"

void ViewportPanel::draw(const EditorLayout& layout, EditorGizmoOperation operation,
    const EditorEntityView* selectedEntity) {
    drawChrome(layout);
    drawToolbar(layout, operation);
    drawPerformanceOverlay(layout);
    drawStatusBar(layout, selectedEntity);
}

void ViewportPanel::drawChrome(const EditorLayout& layout) {
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    const ImVec2 min = layout.viewport.pos;
    const ImVec2 max = ImVec2(
        layout.viewport.pos.x + layout.viewport.size.x,
        layout.viewport.pos.y + layout.viewport.size.y);

    drawList->AddRectFilled(
        min,
        ImVec2(max.x, min.y + 28.0f),
        IM_COL32(12, 14, 18, 210));
    drawList->AddRect(min, max, IM_COL32(42, 48, 58, 255), 0.0f, 0, 1.5f);

    ImGui::SetNextWindowPos(ImVec2(min.x + 10.0f, min.y + 5.0f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);
    constexpr ImGuiWindowFlags titleFlags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoInputs;
    if (ImGui::Begin("##ViewportTitle", nullptr, titleFlags)) {
        ImGui::PushStyleColor(ImGuiCol_Text, EditorTheme::accent());
        ImGui::TextUnformatted("SCENE");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::TextDisabled("Viewport");
    }
    ImGui::End();
}

void ViewportPanel::drawToolbar(const EditorLayout& layout, EditorGizmoOperation operation) {
    ImGui::SetNextWindowPos(
        ImVec2(layout.viewport.pos.x + 12.0f, layout.viewport.pos.y + 36.0f),
        ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.90f);

    constexpr ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoNav;

    if (!ImGui::Begin("##ViewportToolbar", nullptr, flags)) {
        ImGui::End();
        return;
    }

    const char* labels[] = { "Move", "Rotate", "Scale" };
    const EditorGizmoOperation operations[] = {
        EditorGizmoOperation::Translate,
        EditorGizmoOperation::Rotate,
        EditorGizmoOperation::Scale
    };

    for (int i = 0; i < 3; ++i) {
        const bool selected = operation == operations[i];
        if (selected) {
            ImGui::PushStyleColor(ImGuiCol_Button, EditorTheme::accent());
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, EditorTheme::accentHover());
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        }

        if (ImGui::Button(labels[i], ImVec2(78.0f, 0.0f))) {
            EditorEvent event;
            event.type = EditorEventType::SetGizmoOperation;
            event.info.gizmoOperation = operations[i];
            EventSystem::pushEvent(event);
        }

        if (selected) {
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

    constexpr ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav;

    if (ImGui::Begin("##Performance", nullptr, flags)) {
        ImGui::Text("FPS  %.0f", io.Framerate);
        ImGui::TextDisabled("%.2f ms", 1000.0f / io.Framerate);
    }
    ImGui::End();
}

void ViewportPanel::drawStatusBar(const EditorLayout& layout, const EditorEntityView* selectedEntity) {
    const float barHeight = 26.0f;
    const ImVec2 pos = ImVec2(
        layout.viewport.pos.x + 8.0f,
        layout.viewport.pos.y + layout.viewport.size.y - barHeight - 8.0f);

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(layout.viewport.size.x - 16.0f, barHeight), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.88f);

    constexpr ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoInputs;

    if (!ImGui::Begin("##ViewportStatus", nullptr, flags)) {
        ImGui::End();
        return;
    }

    if (selectedEntity) {
        ImGui::Text("%s  ·  ID %u  ·  Mesh %llu",
            selectedEntity->name.c_str(),
            selectedEntity->entityID,
            (unsigned long long)selectedEntity->meshID);
    }
    else {
        ImGui::TextDisabled("Nothing selected");
    }

    ImGui::End();
}
