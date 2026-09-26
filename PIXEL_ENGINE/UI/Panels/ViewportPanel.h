#pragma once

#include "../Layout/EditorLayout.h"
#include "../EditorViews.h"
#include "../../Core/EventSystem/EventTypes.h"

class ViewportPanel {
public:
    void draw(const EditorLayout& layout, EditorGizmoOperation operation,
        const EditorEntityView* selectedEntity);

private:
    void drawChrome(const EditorLayout& layout);
    void drawToolbar(const EditorLayout& layout, EditorGizmoOperation operation);
    void drawPerformanceOverlay(const EditorLayout& layout);
    void drawStatusBar(const EditorLayout& layout, const EditorEntityView* selectedEntity);
};
