#include "Gizmo.h"

#include <cmath>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <ImGuizmo.h>

#include "Core/EventSystem/EventSystem.h"

void Gizmo::setOperation(GizmoOperation operation) {
    m_operation = operation;
}

void Gizmo::draw(const EditorEntityView* pSelectedEntity, const EditorLayout& layout, int screenHeight) {
    if (pSelectedEntity == nullptr) {
        return;
    }

    const float viewport_x = static_cast<float>(layout.viewportX);
    const float viewport_y = static_cast<float>(screenHeight) - static_cast<float>(layout.viewportY) - static_cast<float>(layout.viewportH);
    const float viewport_w = static_cast<float>(layout.viewportW);
    const float viewport_h = static_cast<float>(layout.viewportH);

    if (m_operation == GizmoOperation::Rotate) {
        drawRotateGizmo(*pSelectedEntity, viewport_x, viewport_y, viewport_w, viewport_h);
        return;
    }

    ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
    if (m_operation == GizmoOperation::Scale) {
        operation = ImGuizmo::SCALE;
    }

    ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
    ImGuizmo::SetOrthographic(true);
    ImGuizmo::SetGizmoSizeClipSpace(0.18f);
    ImGuizmo::SetRect(viewport_x, viewport_y, viewport_w, viewport_h);

    ImGuizmo::Style& gizmo_style = ImGuizmo::GetStyle();
    gizmo_style.TranslationLineThickness = 5.0f;
    gizmo_style.TranslationLineArrowSize = 9.0f;
    gizmo_style.RotationLineThickness = 4.0f;
    gizmo_style.RotationOuterLineThickness = 4.0f;
    gizmo_style.ScaleLineThickness = 5.0f;
    gizmo_style.ScaleLineCircleSize = 8.0f;
    gizmo_style.HatchedAxisLineThickness = 0.0f;
    gizmo_style.CenterCircleSize = 7.0f;

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 object_matrix = pSelectedEntity->worldMatrix;
    glm::mat4 delta_matrix = glm::mat4(1.0f);

    const bool manipulated = ImGuizmo::Manipulate(
        glm::value_ptr(view),
        glm::value_ptr(projection),
        operation,
        ImGuizmo::WORLD,
        glm::value_ptr(object_matrix),
        glm::value_ptr(delta_matrix));

    if (!manipulated) {
        return;
    }

    float translation[3] = {};
    float rotation[3] = {};
    float scale[3] = {};
    ImGuizmo::DecomposeMatrixToComponents(
        glm::value_ptr(object_matrix),
        translation,
        rotation,
        scale);

    for (float& scale_val : scale) {
        if (scale_val < 1.0f) {
            scale_val = 1.0f;
        }
    }

    auto push_transform_event = [pSelectedEntity](EditorEventType type,
        const float* pNewTranslation,
        const float* pNewRotation,
        const float* pNewScale) {
        EditorEvent event;
        event.type = type;
        event.info.entityID = pSelectedEntity->entityID;
        event.info.position = EditorVec3{ pNewTranslation[0], pNewTranslation[1], pNewTranslation[2] };
        event.info.rotation = EditorVec3{ pNewRotation[0], pNewRotation[1], pNewRotation[2] };
        event.info.scale = EditorVec3{ pNewScale[0], pNewScale[1], pNewScale[2] };
        EventSystem::pushEvent(event);
    };

    const float current_rotation[3] = {
        pSelectedEntity->rotation.x,
        pSelectedEntity->rotation.y,
        pSelectedEntity->rotation.z
    };
    const float current_translation[3] = {
        pSelectedEntity->position.x,
        pSelectedEntity->position.y,
        pSelectedEntity->position.z
    };
    const float current_scale[3] = {
        pSelectedEntity->scale.x < 1.0f ? 1.0f : pSelectedEntity->scale.x,
        pSelectedEntity->scale.y < 1.0f ? 1.0f : pSelectedEntity->scale.y,
        pSelectedEntity->scale.z < 1.0f ? 1.0f : pSelectedEntity->scale.z
    };

    if (m_operation == GizmoOperation::Translate) {
        push_transform_event(EditorEventType::Translate, translation, current_rotation, current_scale);
    }
    else {
        push_transform_event(EditorEventType::Scale, current_translation, current_rotation, scale);
    }
}

void Gizmo::drawRotateGizmo(const EditorEntityView& entity, float viewportX, float viewportY,
    float viewportWidth, float viewportHeight) {
    const glm::mat4& object_matrix = entity.worldMatrix;
    glm::vec4 clip_center = object_matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    if (clip_center.w != 0.0f) {
        clip_center.x /= clip_center.w;
        clip_center.y /= clip_center.w;
    }

    const ImVec2 center(
        viewportX + (clip_center.x * 0.5f + 0.5f) * viewportWidth,
        viewportY + (0.5f - clip_center.y * 0.5f) * viewportHeight);

    constexpr int AXIS_COUNT = 3;
    constexpr int SEGMENT_COUNT = 96;
    constexpr float RADIUS = 88.0f;
    const ImU32 colors[AXIS_COUNT] = {
        IM_COL32(230, 65, 65, 255),
        IM_COL32(65, 210, 95, 255),
        IM_COL32(85, 140, 255, 255)
    };

    auto make_ring_point = [center, RADIUS](int axis, float angle) {
        const float c = std::cos(angle);
        const float s = std::sin(angle);

        switch (axis) {
        case 0:
            return ImVec2(
                center.x + c * RADIUS * 0.34f + s * RADIUS * 0.12f,
                center.y + c * RADIUS * 0.10f - s * RADIUS);
        case 1:
            return ImVec2(
                center.x + c * RADIUS,
                center.y + c * RADIUS * 0.08f + s * RADIUS * 0.34f);
        default:
            return ImVec2(
                center.x + c * RADIUS,
                center.y + s * RADIUS);
        }
    };

    auto distance_to_segment = [](const ImVec2& p, const ImVec2& a, const ImVec2& b) {
        const float abx = b.x - a.x;
        const float aby = b.y - a.y;
        const float apx = p.x - a.x;
        const float apy = p.y - a.y;
        const float ab_length_sq = abx * abx + aby * aby;

        float t = 0.0f;
        if (ab_length_sq > 0.0f) {
            t = (apx * abx + apy * aby) / ab_length_sq;
            if (t < 0.0f) {
                t = 0.0f;
            }
            else if (t > 1.0f) {
                t = 1.0f;
            }
        }

        const float closest_x = a.x + abx * t;
        const float closest_y = a.y + aby * t;
        const float dx = p.x - closest_x;
        const float dy = p.y - closest_y;
        return std::sqrt(dx * dx + dy * dy);
    };

    ImDrawList* p_draw_list = ImGui::GetForegroundDrawList();
    for (int axis = 0; axis < AXIS_COUNT; ++axis) {
        const bool is_active = (m_activeRotateAxis == axis);

        ImVec2 points[SEGMENT_COUNT + 1];
        for (int segment = 0; segment <= SEGMENT_COUNT; ++segment) {
            const float angle = static_cast<float>(segment) / static_cast<float>(SEGMENT_COUNT) * 6.283185307f;
            points[segment] = make_ring_point(axis, angle);
        }

        p_draw_list->AddPolyline(
            points,
            SEGMENT_COUNT + 1,
            is_active ? IM_COL32(255, 255, 255, 255) : colors[axis],
            ImDrawFlags_Closed,
            is_active ? 5.5f : 4.0f);
    }

    ImGuiIO& io = ImGui::GetIO();
    const ImVec2 mouse = io.MousePos;
    const float dx = mouse.x - center.x;
    const float dy = mouse.y - center.y;

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        int closest_axis = -1;
        float closest_dist = 99999.0f;

        for (int axis = 0; axis < AXIS_COUNT; ++axis) {
            float axis_dist = 99999.0f;
            ImVec2 prev_point = make_ring_point(axis, 0.0f);

            for (int segment = 1; segment <= SEGMENT_COUNT; ++segment) {
                const float angle = static_cast<float>(segment) / static_cast<float>(SEGMENT_COUNT) * 6.283185307f;
                const ImVec2 cur_point = make_ring_point(axis, angle);
                const float seg_dist = distance_to_segment(mouse, prev_point, cur_point);
                if (seg_dist < axis_dist) {
                    axis_dist = seg_dist;
                }
                prev_point = cur_point;
            }

            if (axis_dist < 12.0f && axis_dist < closest_dist) {
                closest_dist = axis_dist;
                closest_axis = axis;
            }
        }

        if (closest_axis >= 0) {
            m_activeRotateAxis = closest_axis;
            m_rotateDragStartAngle = std::atan2(dy, dx);
            m_rotateDragStartMouse[0] = mouse.x;
            m_rotateDragStartMouse[1] = mouse.y;
            m_rotateDragStart[0] = entity.rotation.x;
            m_rotateDragStart[1] = entity.rotation.y;
            m_rotateDragStart[2] = entity.rotation.z;
        }
    }

    if (m_activeRotateAxis >= 0 && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        constexpr float RADIANS_TO_DEGREES = 57.2957795131f;
        constexpr float PI_VAL = 3.1415926535f;

        float new_rotation[3] = {
            m_rotateDragStart[0],
            m_rotateDragStart[1],
            m_rotateDragStart[2]
        };

        if (m_activeRotateAxis == 0) {
            new_rotation[0] += (m_rotateDragStartMouse[1] - mouse.y) * 0.6f;
        }
        else if (m_activeRotateAxis == 1) {
            new_rotation[1] += (mouse.x - m_rotateDragStartMouse[0]) * 0.6f;
        }
        else {
            float delta_radians = std::atan2(dy, dx) - m_rotateDragStartAngle;
            if (delta_radians > PI_VAL) {
                delta_radians -= 2.0f * PI_VAL;
            }
            else if (delta_radians < -PI_VAL) {
                delta_radians += 2.0f * PI_VAL;
            }
            new_rotation[2] += delta_radians * RADIANS_TO_DEGREES;
        }

        const float cur_translation[3] = {
            entity.position.x,
            entity.position.y,
            entity.position.z
        };
        const float cur_scale[3] = {
            entity.scale.x < 1.0f ? 1.0f : entity.scale.x,
            entity.scale.y < 1.0f ? 1.0f : entity.scale.y,
            entity.scale.z < 1.0f ? 1.0f : entity.scale.z
        };

        EditorEvent rotate_event;
        rotate_event.type = EditorEventType::Rotate;
        rotate_event.info.entityID = entity.entityID;
        rotate_event.info.position = EditorVec3{ cur_translation[0], cur_translation[1], cur_translation[2] };
        rotate_event.info.rotation = EditorVec3{ new_rotation[0], new_rotation[1], new_rotation[2] };
        rotate_event.info.scale = EditorVec3{ cur_scale[0], cur_scale[1], cur_scale[2] };
        EventSystem::pushEvent(rotate_event);
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        m_activeRotateAxis = -1;
    }
}
