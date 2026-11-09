#pragma once

#include "Core/Definitions/CommonDefinitions.h"
#include "Systems/UI/EditorViews.h"
#include "Systems/UI/Layout/EditorLayout.h"

class Gizmo {
public:
    void setOperation(GizmoOperation operation);
    GizmoOperation getOperation() const {
        return m_operation;
    }

    void draw(const EditorEntityView* pSelectedEntity, const EditorCameraView* pCamera,
        const EditorLayout& layout);

private:
    GizmoOperation m_operation = GizmoOperation::Translate;
};
