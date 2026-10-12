#pragma once

#include "Core/Definitions/CommonDefinitions.h"
#include "Core/EventSystem/EventTypes.h"
#include "Systems/UI/EditorViews.h"
#include "Systems/UI/Layout/EditorLayout.h"

class Gizmo {
public:
    void setOperation(GizmoOperation operation);
    GizmoOperation getOperation() const {
        return m_operation;
    }

    void draw(const EditorEntityView* pSelectedEntity, const EditorLayout& layout, int screenHeight);

private:
    GizmoOperation m_operation = GizmoOperation::Translate;
    int m_activeRotateAxis = -1;
    float m_rotateDragStartAngle = 0.0f;
    float m_rotateDragStartMouse[2] = { 0.0f, 0.0f };
    float m_rotateDragStart[3] = { 0.0f, 0.0f, 0.0f };

    void drawRotateGizmo(const EditorEntityView& entity, float viewportX, float viewportY,
        float viewportWidth, float viewportHeight);
};
