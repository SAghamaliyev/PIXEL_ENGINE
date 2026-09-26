#pragma once

#include "../../Core/EventSystem/EventTypes.h"
#include "../Layout/EditorLayout.h"
#include "../EditorViews.h"

class Gizmo {
public:
    void setOperation(EditorGizmoOperation operation);
    EditorGizmoOperation getOperation() const { return m_operation; }

    void draw(const EditorEntityView* selected, const EditorLayout& layout, int screenH);

private:
    EditorGizmoOperation m_operation = EditorGizmoOperation::Translate;
    int m_activeRotateAxis = -1;
    float m_rotateDragStartAngle = 0.0f;
    float m_rotateDragStartMouse[2] = { 0.0f, 0.0f };
    float m_rotateDragStart[3] = { 0.0f, 0.0f, 0.0f };

    void drawRotateGizmo(const EditorEntityView& entity, float viewportX, float viewportY,
        float viewportW, float viewportH);
};
