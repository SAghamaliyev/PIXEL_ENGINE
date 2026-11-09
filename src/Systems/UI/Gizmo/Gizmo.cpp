#include "Gizmo.h"

#include <algorithm>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <ImGuizmo.h>

#include "Core/EventSystem/EventSystem.h"

namespace {

constexpr float kGizmoSizeClipSpace = 0.18f;

bool isMouseInViewport(const EditorLayout& layout) {
    const ImVec2 mouse = ImGui::GetIO().MousePos;
    const ImVec2 min_pos = layout.viewport.pos;
    const ImVec2 max_pos = ImVec2(
        min_pos.x + layout.viewport.size.x,
        min_pos.y + layout.viewport.size.y);

    return mouse.x >= min_pos.x && mouse.x < max_pos.x &&
        mouse.y >= min_pos.y && mouse.y < max_pos.y;
}

ImGuizmo::OPERATION toImGuizmoOperation(GizmoOperation operation) {
    switch (operation) {
    case GizmoOperation::Rotate:
        return ImGuizmo::ROTATE;
    case GizmoOperation::Scale:
        return ImGuizmo::SCALE;
    case GizmoOperation::Translate:
    default:
        return ImGuizmo::TRANSLATE;
    }
}

ImGuizmo::MODE toImGuizmoMode(GizmoOperation operation) {
    if (operation == GizmoOperation::Translate) {
        return ImGuizmo::WORLD;
    }
    return ImGuizmo::LOCAL;
}

glm::vec3 extractScale(const glm::mat4& matrix) {
    return glm::vec3(
        glm::length(glm::vec3(matrix[0])),
        glm::length(glm::vec3(matrix[1])),
        glm::length(glm::vec3(matrix[2])));
}

bool isValidScale(const glm::vec3& scale) {
    for (int i = 0; i < 3; ++i) {
        if (!std::isfinite(scale[i]) || std::abs(scale[i]) < 0.0001f) {
            return false;
        }
    }
    return true;
}

float wrapAngleDelta(float previous_degrees, float next_degrees) {
    float delta = next_degrees - previous_degrees;
    while (delta > 180.0f) {
        next_degrees -= 360.0f;
        delta -= 360.0f;
    }
    while (delta < -180.0f) {
        next_degrees += 360.0f;
        delta += 360.0f;
    }
    return next_degrees;
}

// Inverse of TransformManager: R = Rx * Ry * Rz, Euler stored in degrees.
// GLM is column-major: matrix[col][row].
// For R = Rx * Ry * Rz the key elements are:
//   R[2][0] =  sin(Y)
//   R[2][1] = -sin(X)*cos(Y)   R[2][2] = cos(X)*cos(Y)
//   R[1][0] = -sin(Z)*cos(Y)   R[0][0] = cos(Z)*cos(Y)
glm::vec3 extractEulerDegreesXYZ(const glm::mat4& matrix, const glm::vec3& previous_degrees) {
    const glm::vec3 scale = extractScale(matrix);
    glm::mat3 rotation{glm::vec3(matrix[0]), glm::vec3(matrix[1]), glm::vec3(matrix[2])};
    if (isValidScale(scale)) {
        rotation[0] /= scale.x;
        rotation[1] /= scale.y;
        rotation[2] /= scale.z;
    }

    glm::vec3 radians;
    const float sy = glm::clamp(rotation[2][0], -1.0f, 1.0f);
    radians.y = std::asin(sy);
    if (std::abs(sy) < 0.9999f) {
        radians.x = std::atan2(-rotation[2][1], rotation[2][2]);
        radians.z = std::atan2(-rotation[1][0], rotation[0][0]);
    }
    else {
        radians.x = std::atan2(rotation[0][1], rotation[1][1]);
        radians.z = 0.0f;
    }

    glm::vec3 degrees = glm::degrees(radians);
    degrees.x = wrapAngleDelta(previous_degrees.x, degrees.x);
    degrees.y = wrapAngleDelta(previous_degrees.y, degrees.y);
    degrees.z = wrapAngleDelta(previous_degrees.z, degrees.z);
    return degrees;
}

void pushTransformEvent(EditorEventType type, const EditorEntityView& entity,
    const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale) {
    EditorEvent event;
    event.type = type;
    event.info.entityID = entity.entityID;
    event.info.translation = translation;
    event.info.rotation = rotation;
    event.info.scale = scale;
    EventSystem::pushEvent(event);
}

} // namespace

void Gizmo::setOperation(GizmoOperation operation) {
    m_operation = operation;
}

void Gizmo::draw(const EditorEntityView* pSelectedEntity, const EditorCameraView* pCamera,
    const EditorLayout& layout) {
    if (pSelectedEntity == nullptr || pCamera == nullptr) {
        return;
    }

    if (layout.viewport.size.x <= 0.0f || layout.viewport.size.y <= 0.0f) {
        return;
    }

    const ImVec2 viewport_min = layout.viewport.pos;
    const ImVec2 viewport_max = ImVec2(
        viewport_min.x + layout.viewport.size.x,
        viewport_min.y + layout.viewport.size.y);

    ImDrawList* p_draw_list = ImGui::GetForegroundDrawList();
    p_draw_list->PushClipRect(viewport_min, viewport_max, true);

    ImGuizmo::SetDrawlist(p_draw_list);
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetGizmoSizeClipSpace(kGizmoSizeClipSpace);
    ImGuizmo::SetRect(
        viewport_min.x,
        viewport_min.y,
        layout.viewport.size.x,
        layout.viewport.size.y);

    ImGuizmo::Style& gizmo_style = ImGuizmo::GetStyle();
    gizmo_style.TranslationLineThickness = 5.0f;
    gizmo_style.TranslationLineArrowSize = 9.0f;
    gizmo_style.RotationLineThickness = 4.0f;
    gizmo_style.RotationOuterLineThickness = 4.0f;
    gizmo_style.ScaleLineThickness = 5.0f;
    gizmo_style.ScaleLineCircleSize = 8.0f;
    gizmo_style.HatchedAxisLineThickness = 0.0f;
    gizmo_style.CenterCircleSize = 7.0f;

    const bool allow_interaction = ImGuizmo::IsUsing() ||
        (isMouseInViewport(layout) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow));

    ImGuizmo::Enable(allow_interaction);

    glm::mat4 object_matrix = pSelectedEntity->worldMatrix;

    const bool manipulated = ImGuizmo::Manipulate(
        glm::value_ptr(pCamera->view),
        glm::value_ptr(pCamera->projection),
        toImGuizmoOperation(m_operation),
        toImGuizmoMode(m_operation),
        glm::value_ptr(object_matrix));

    p_draw_list->PopClipRect();

    if (!manipulated) {
        return;
    }

    if (m_operation == GizmoOperation::Translate) {
        const glm::vec3 translation = glm::vec3(object_matrix[3]);
        pushTransformEvent(EditorEventType::Translate, *pSelectedEntity,
            translation, pSelectedEntity->rotation, pSelectedEntity->scale);
        return;
    }

    if (m_operation == GizmoOperation::Rotate) {
        const glm::vec3 rotation = extractEulerDegreesXYZ(object_matrix, pSelectedEntity->rotation);
        pushTransformEvent(EditorEventType::Rotate, *pSelectedEntity,
            pSelectedEntity->position, rotation, pSelectedEntity->scale);
        return;
    }

    const glm::vec3 scale = extractScale(object_matrix);
    if (!isValidScale(scale)) {
        return;
    }
    pushTransformEvent(EditorEventType::Scale, *pSelectedEntity,
        pSelectedEntity->position, pSelectedEntity->rotation, scale);
}
