#include "Gizmo.h"

#include "../Materials/imgui.h"
#include "../../Core/EventSystem/EventSystem.h"
#include "../../dependencies/ImGuizmo.h"
#include "../../dependencies/glm-1.0.3/glm/gtc/type_ptr.hpp"

#include <cmath>

void Gizmo::setOperation(EditorGizmoOperation operation) {
    m_operation = operation;
}

void Gizmo::draw(const EditorEntityView* selected, const EditorLayout& layout, int screenH) {
    if (!selected) {
        return;
    }

    const float viewportX = (float)layout.viewportX;
    const float viewportY = (float)screenH - (float)layout.viewportY - (float)layout.viewportH;
    const float viewportW = (float)layout.viewportW;
    const float viewportH = (float)layout.viewportH;

    if (m_operation == EditorGizmoOperation::Rotate) {
        drawRotateGizmo(*selected, viewportX, viewportY, viewportW, viewportH);
        return;
    }

    ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
    if (m_operation == EditorGizmoOperation::Scale) {
        operation = ImGuizmo::SCALE;
    }

    ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
    ImGuizmo::SetOrthographic(true);
    ImGuizmo::SetGizmoSizeClipSpace(0.18f);
    ImGuizmo::SetRect(viewportX, viewportY, viewportW, viewportH);

    ImGuizmo::Style& gizmoStyle = ImGuizmo::GetStyle();
    gizmoStyle.TranslationLineThickness = 5.0f;
    gizmoStyle.TranslationLineArrowSize = 9.0f;
    gizmoStyle.RotationLineThickness = 4.0f;
    gizmoStyle.RotationOuterLineThickness = 4.0f;
    gizmoStyle.ScaleLineThickness = 5.0f;
    gizmoStyle.ScaleLineCircleSize = 8.0f;
    gizmoStyle.HatchedAxisLineThickness = 0.0f;
    gizmoStyle.CenterCircleSize = 7.0f;

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 objectMatrix = selected->worldMatrix;
    glm::mat4 deltaMatrix = glm::mat4(1.0f);

    const bool manipulated = ImGuizmo::Manipulate(
        glm::value_ptr(view),
        glm::value_ptr(projection),
        operation,
        ImGuizmo::WORLD,
        glm::value_ptr(objectMatrix),
        glm::value_ptr(deltaMatrix));

    if (!manipulated) {
        return;
    }

    float translation[3] = {};
    float rotation[3] = {};
    float scale[3] = {};
    ImGuizmo::DecomposeMatrixToComponents(
        glm::value_ptr(objectMatrix),
        translation,
        rotation,
        scale);

    for (float& scaleValue : scale) {
        if (scaleValue < 1.0f) {
            scaleValue = 1.0f;
        }
    }

    auto pushTransformEvent = [selected](EditorEventType type,
        const float* newTranslation,
        const float* newRotation,
        const float* newScale) {
        EditorEvent event;
        event.type = type;
        event.info.entityID = selected->entityID;
        event.info.position = EditorVec3{ newTranslation[0], newTranslation[1], newTranslation[2] };
        event.info.rotation = EditorVec3{ newRotation[0], newRotation[1], newRotation[2] };
        event.info.scale = EditorVec3{ newScale[0], newScale[1], newScale[2] };
        EventSystem::pushEvent(event);
    };

    const float currentRotation[3] = {
        selected->rotation.x,
        selected->rotation.y,
        selected->rotation.z
    };
    const float currentTranslation[3] = {
        selected->position.x,
        selected->position.y,
        selected->position.z
    };
    const float currentScale[3] = {
        selected->scale.x < 1.0f ? 1.0f : selected->scale.x,
        selected->scale.y < 1.0f ? 1.0f : selected->scale.y,
        selected->scale.z < 1.0f ? 1.0f : selected->scale.z
    };

    if (m_operation == EditorGizmoOperation::Translate) {
        pushTransformEvent(EditorEventType::Translate, translation, currentRotation, currentScale);
    }
    else {
        pushTransformEvent(EditorEventType::Scale, currentTranslation, currentRotation, scale);
    }
}

void Gizmo::drawRotateGizmo(const EditorEntityView& entity, float viewportX, float viewportY,
    float viewportW, float viewportH) {
    const glm::mat4& objectMatrix = entity.worldMatrix;
    glm::vec4 clipCenter = objectMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    if (clipCenter.w != 0.0f) {
        clipCenter.x /= clipCenter.w;
        clipCenter.y /= clipCenter.w;
    }

    const ImVec2 center(
        viewportX + (clipCenter.x * 0.5f + 0.5f) * viewportW,
        viewportY + (0.5f - clipCenter.y * 0.5f) * viewportH);

    constexpr int axisCount = 3;
    constexpr int segmentCount = 96;
    const float radius = 88.0f;
    const ImU32 colors[axisCount] = {
        IM_COL32(230, 65, 65, 255),
        IM_COL32(65, 210, 95, 255),
        IM_COL32(85, 140, 255, 255)
    };

    auto makeRingPoint = [center, radius](int axis, float angle) {
        const float c = std::cos(angle);
        const float s = std::sin(angle);

        switch (axis) {
        case 0:
            return ImVec2(
                center.x + c * radius * 0.34f + s * radius * 0.12f,
                center.y + c * radius * 0.10f - s * radius);
        case 1:
            return ImVec2(
                center.x + c * radius,
                center.y + c * radius * 0.08f + s * radius * 0.34f);
        default:
            return ImVec2(
                center.x + c * radius,
                center.y + s * radius);
        }
    };

    auto distanceToSegment = [](const ImVec2& p, const ImVec2& a, const ImVec2& b) {
        const float abx = b.x - a.x;
        const float aby = b.y - a.y;
        const float apx = p.x - a.x;
        const float apy = p.y - a.y;
        const float abLengthSq = abx * abx + aby * aby;

        float t = 0.0f;
        if (abLengthSq > 0.0f) {
            t = (apx * abx + apy * aby) / abLengthSq;
            if (t < 0.0f) {
                t = 0.0f;
            }
            else if (t > 1.0f) {
                t = 1.0f;
            }
        }

        const float closestX = a.x + abx * t;
        const float closestY = a.y + aby * t;
        const float dx = p.x - closestX;
        const float dy = p.y - closestY;
        return std::sqrt(dx * dx + dy * dy);
    };

    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    for (int axis = 0; axis < axisCount; ++axis) {
        const bool active = (m_activeRotateAxis == axis);

        ImVec2 points[segmentCount + 1];
        for (int segment = 0; segment <= segmentCount; ++segment) {
            const float angle = (float)segment / (float)segmentCount * 6.283185307f;
            points[segment] = makeRingPoint(axis, angle);
        }

        drawList->AddPolyline(
            points,
            segmentCount + 1,
            active ? IM_COL32(255, 255, 255, 255) : colors[axis],
            ImDrawFlags_Closed,
            active ? 5.5f : 4.0f);
    }

    ImGuiIO& io = ImGui::GetIO();
    const ImVec2 mouse = io.MousePos;
    const float dx = mouse.x - center.x;
    const float dy = mouse.y - center.y;

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        int closestAxis = -1;
        float closestDistance = 99999.0f;

        for (int axis = 0; axis < axisCount; ++axis) {
            float axisDistance = 99999.0f;
            ImVec2 previousPoint = makeRingPoint(axis, 0.0f);

            for (int segment = 1; segment <= segmentCount; ++segment) {
                const float angle = (float)segment / (float)segmentCount * 6.283185307f;
                const ImVec2 currentPoint = makeRingPoint(axis, angle);
                const float segmentDistance = distanceToSegment(mouse, previousPoint, currentPoint);
                if (segmentDistance < axisDistance) {
                    axisDistance = segmentDistance;
                }
                previousPoint = currentPoint;
            }

            if (axisDistance < 12.0f && axisDistance < closestDistance) {
                closestDistance = axisDistance;
                closestAxis = axis;
            }
        }

        if (closestAxis >= 0) {
            m_activeRotateAxis = closestAxis;
            m_rotateDragStartAngle = std::atan2(dy, dx);
            m_rotateDragStartMouse[0] = mouse.x;
            m_rotateDragStartMouse[1] = mouse.y;
            m_rotateDragStart[0] = entity.rotation.x;
            m_rotateDragStart[1] = entity.rotation.y;
            m_rotateDragStart[2] = entity.rotation.z;
        }
    }

    if (m_activeRotateAxis >= 0 && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        constexpr float radiansToDegrees = 57.2957795131f;
        constexpr float pi = 3.1415926535f;

        float newRotation[3] = {
            m_rotateDragStart[0],
            m_rotateDragStart[1],
            m_rotateDragStart[2]
        };

        if (m_activeRotateAxis == 0) {
            newRotation[0] += (m_rotateDragStartMouse[1] - mouse.y) * 0.6f;
        }
        else if (m_activeRotateAxis == 1) {
            newRotation[1] += (mouse.x - m_rotateDragStartMouse[0]) * 0.6f;
        }
        else {
            float deltaRadians = std::atan2(dy, dx) - m_rotateDragStartAngle;
            if (deltaRadians > pi) {
                deltaRadians -= 2.0f * pi;
            }
            else if (deltaRadians < -pi) {
                deltaRadians += 2.0f * pi;
            }
            newRotation[2] += deltaRadians * radiansToDegrees;
        }

        const float currentTranslation[3] = {
            entity.position.x,
            entity.position.y,
            entity.position.z
        };
        const float currentScale[3] = {
            entity.scale.x < 1.0f ? 1.0f : entity.scale.x,
            entity.scale.y < 1.0f ? 1.0f : entity.scale.y,
            entity.scale.z < 1.0f ? 1.0f : entity.scale.z
        };

        EditorEvent event;
        event.type = EditorEventType::Rotate;
        event.info.entityID = entity.entityID;
        event.info.position = EditorVec3{ currentTranslation[0], currentTranslation[1], currentTranslation[2] };
        event.info.rotation = EditorVec3{ newRotation[0], newRotation[1], newRotation[2] };
        event.info.scale = EditorVec3{ currentScale[0], currentScale[1], currentScale[2] };
        EventSystem::pushEvent(event);
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        m_activeRotateAxis = -1;
    }
}
