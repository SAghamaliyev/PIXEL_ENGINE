#pragma once

#include "Core/Definitions/CommonDefinitions.h"
#include "Systems/UI/EditorViews.h"
#include "Systems/UI/Layout/EditorLayout.h"

class ViewportPanel {
public:
    void draw(const EditorLayout& layout, GizmoOperation operation, const EditorEntityView* pSelectedEntity);

private:
    void drawChrome(const EditorLayout& layout);
    void drawToolbar(const EditorLayout& layout, GizmoOperation operation);
    void drawPerformanceOverlay(const EditorLayout& layout);
    void drawStatusBar(const EditorLayout& layout, const EditorEntityView* pSelectedEntity);
};
