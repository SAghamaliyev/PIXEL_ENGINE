#include "EditorUI.h"
#include "EditorTheme.h"
#include "../SceneSystem/SceneSystem.h"
#include "../Core/Logger/Logger.h"

#include "Materials/imgui.h"
#include "Materials/Backends/imgui_impl_glfw.h"
#include "Materials/Backends/imgui_impl_opengl3.h"
#include "../dependencies/ImGuizmo.h"

#include <GLFW/glfw3.h>
#include "../dependencies/glm-1.0.3/glm/gtc/type_ptr.hpp"

#include <cmath>

void EditorUI::init(GLFWwindow* window, void* engineEventTarget) {
    m_window = window;
    m_sceneSystem = static_cast<SceneSystem*>(engineEventTarget);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontDefault();
    io.FontGlobalScale = 1.3f;

    EditorTheme::setDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_hierarchy.setEntityViews(&m_entityViews);
    m_inspector.setEntityViews(&m_entityViews);
    m_inspector.setContentBrowserPanel(&m_contentBrowser);
    m_contentBrowser.setConsole(&m_console);

    Logger::addLog(LOG_INFO, "Engine initialized");
}

void EditorUI::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void EditorUI::render() {
    updateLayout();
    drawMainMenuBar();
    drawPanels();
}

void EditorUI::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorUI::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int EditorUI::getSelectedEntityID() const {
    return m_hierarchy.getSelectedEntityID();
}

void EditorUI::setEntityViews(const std::vector<EditorEntityView>& entityViews) {
    m_entityViews = entityViews;
    m_hierarchy.setEntityViews(&m_entityViews);
    m_inspector.setEntityViews(&m_entityViews);
}

void EditorUI::getViewportRect(int& outX, int& outY, int& outW, int& outH) {
    if (!m_window) {
        return;
    }

    int screenW = 0;
    int screenH = 0;
    glfwGetFramebufferSize(m_window, &screenW, &screenH);

    const EditorLayout layout = computeEditorLayout(screenW, screenH, ImGui::GetFrameHeight());
    outX = layout.viewportX;
    outY = layout.viewportY;
    outW = layout.viewportW;
    outH = layout.viewportH;
}

void EditorUI::updateLayout() {
    int screenW = 0;
    int screenH = 0;
    glfwGetFramebufferSize(m_window, &screenW, &screenH);
    m_layout = computeEditorLayout(screenW, screenH, ImGui::GetFrameHeight());
}

void EditorUI::drawMainMenuBar() {
    if (!ImGui::BeginMainMenuBar()) {
        return;
    }

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
            // /FLAG ClearScene: UI asks the engine to clear the active scene.
            EventSystem::pushEvent(EditorEvent{ EditorEventType::ClearScene });
        }
        if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
            // /FLAG OpenScene: UI asks the engine to open a scene picker/load flow.
            EventSystem::pushEvent(EditorEvent{ EditorEventType::OpenScene });
        }
        if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
            // /FLAG SaveScene: UI asks the engine to save the current scene.
            EventSystem::pushEvent(EditorEvent{ EditorEventType::SaveScene });
        }
        if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) {
            // /FLAG SaveSceneAs: UI asks the engine to save the scene with a new path.
            EventSystem::pushEvent(EditorEvent{ EditorEventType::SaveSceneAs });
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit", "Alt+F4")) {
            m_windowShouldClose = true;
            if (m_window) {
                glfwSetWindowShouldClose(m_window, GLFW_TRUE);
            }
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
            // TODO: undo the last editor action.
        }
        if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
            // TODO: redo the last undone action.
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Preferences")) {
            // TODO: open editor preferences.
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
        bool showHierarchy = m_hierarchy.isVisible();
        if (ImGui::MenuItem("Scene Hierarchy", nullptr, &showHierarchy)) {
            m_hierarchy.setVisible(showHierarchy);
        }

        bool showInspector = m_inspector.isVisible();
        if (ImGui::MenuItem("Inspector", nullptr, &showInspector)) {
            m_inspector.setVisible(showInspector);
        }

        bool showContentBrowser = m_contentBrowser.isVisible();
        if (ImGui::MenuItem("Content Browser", nullptr, &showContentBrowser)) {
            m_contentBrowser.setVisible(showContentBrowser);
        }

        bool showConsole = m_console.isVisible();
        if (ImGui::MenuItem("Console", nullptr, &showConsole)) {
            m_console.setVisible(showConsole);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Theme")) {
        if (ImGui::MenuItem("Dark Theme")) {
            EditorTheme::setDark();
        }
        if (ImGui::MenuItem("Blue Theme")) {
            EditorTheme::setBlue();
        }
        if (ImGui::MenuItem("Purple/Pink Theme")) {
            EditorTheme::setPurple();
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help")) {
        if (ImGui::MenuItem("About")) {
            // /FLAG ShowAbout: UI asks the engine/app layer to show or log the about message.
            EditorEvent event;
            event.type = EditorEventType::ShowAbout;
            event.info.message = "Hello, this is Pixel Engine, i am glad you downloaded it.\n"
                "If you want to add something or noticed some problems\n"
                "please dont be hesitated to contact me: saidaghamaliyev@gmail.com\n"
                "Please ENJOY, Said Aghamaliyev";
            EventSystem::pushEvent(event);
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void EditorUI::drawPanels() {
    m_hierarchy.draw(m_layout);

    m_inspector.setSelectedEntityID(m_hierarchy.getSelectedEntityID());
    m_inspector.setGizmoOperation(m_gizmoOperation);
    m_inspector.draw(m_layout);

    m_contentBrowser.draw(m_layout);
    m_console.draw(m_layout);
    drawGizmo();
    drawPerformanceOverlay();
}

const EditorEntityView* EditorUI::findSelectedEntityView() const {
    const int selectedEntityID = m_hierarchy.getSelectedEntityID();
    if (selectedEntityID < 0) {
        return nullptr;
    }

    for (const EditorEntityView& entity : m_entityViews) {
        if (entity.entityID == (unsigned int)selectedEntityID) {
            return &entity;
        }
    }

    return nullptr;
}

void EditorUI::drawRotateGizmo(const Transform& transform, unsigned int entityID,
    float viewportX, float viewportY, float viewportW, float viewportH) {
    glm::vec4 clipCenter = transform.OurMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
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
            m_rotateDragStart[0] = transform.RotateV.x;
            m_rotateDragStart[1] = transform.RotateV.y;
            m_rotateDragStart[2] = transform.RotateV.z;
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
            transform.TranslateV.x,
            transform.TranslateV.y,
            transform.TranslateV.z
        };
        const float currentScale[3] = {
            transform.ScaleV.x < 1.0f ? 1.0f : transform.ScaleV.x,
            transform.ScaleV.y < 1.0f ? 1.0f : transform.ScaleV.y,
            transform.ScaleV.z < 1.0f ? 1.0f : transform.ScaleV.z
        };

        EditorEvent event;
        event.type = EditorEventType::Rotate;
        event.info.entityID = entityID;
        event.info.position = EditorVec3{ currentTranslation[0], currentTranslation[1], currentTranslation[2] };
        event.info.rotation = EditorVec3{ newRotation[0], newRotation[1], newRotation[2] };
        event.info.scale = EditorVec3{ currentScale[0], currentScale[1], currentScale[2] };
        EventSystem::pushEvent(event);
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        m_activeRotateAxis = -1;
    }
}

void EditorUI::drawGizmo() {
    if (!m_sceneSystem || !m_window) {
        return;
    }

    const EditorEntityView* selectedEntity = findSelectedEntityView();
    if (!selectedEntity) {
        return;
    }

    int screenW = 0;
    int screenH = 0;
    glfwGetFramebufferSize(m_window, &screenW, &screenH);

    const float viewportX = (float)m_layout.viewportX;
    const float viewportY = (float)screenH - (float)m_layout.viewportY - (float)m_layout.viewportH;
    const float viewportW = (float)m_layout.viewportW;
    const float viewportH = (float)m_layout.viewportH;

    const Transform& transform = m_sceneSystem->getTransformInfoEntity(selectedEntity->entityID);
    if (m_gizmoOperation == EditorGizmoOperation::Rotate) {
        drawRotateGizmo(transform, selectedEntity->entityID, viewportX, viewportY, viewportW, viewportH);
        return;
    }

    ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
    switch (m_gizmoOperation) {
    case EditorGizmoOperation::Translate:
        operation = ImGuizmo::TRANSLATE;
        break;
    case EditorGizmoOperation::Rotate:
        operation = ImGuizmo::ROTATE;
        break;
    case EditorGizmoOperation::Scale:
        operation = ImGuizmo::SCALE;
        break;
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
    glm::mat4 objectMatrix = transform.OurMatrix;
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

    float deltaTranslation[3] = {};
    float deltaRotation[3] = {};
    float deltaScale[3] = {};
    ImGuizmo::DecomposeMatrixToComponents(
        glm::value_ptr(deltaMatrix),
        deltaTranslation,
        deltaRotation,
        deltaScale);

    auto pushTransformEvent = [selectedEntity](EditorEventType type,
        const float* newTranslation,
        const float* newRotation,
        const float* newScale) {
        EditorEvent event;
        event.type = type;
        event.info.entityID = selectedEntity->entityID;
        event.info.position = EditorVec3{ newTranslation[0], newTranslation[1], newTranslation[2] };
        event.info.rotation = EditorVec3{ newRotation[0], newRotation[1], newRotation[2] };
        event.info.scale = EditorVec3{ newScale[0], newScale[1], newScale[2] };
        EventSystem::pushEvent(event);
    };

    switch (m_gizmoOperation) {
    case EditorGizmoOperation::Translate: {
        const float currentRotation[3] = {
            transform.RotateV.x,
            transform.RotateV.y,
            transform.RotateV.z
        };
        const float currentScale[3] = {
            transform.ScaleV.x < 1.0f ? 1.0f : transform.ScaleV.x,
            transform.ScaleV.y < 1.0f ? 1.0f : transform.ScaleV.y,
            transform.ScaleV.z < 1.0f ? 1.0f : transform.ScaleV.z
        };
        pushTransformEvent(EditorEventType::Translate, translation, currentRotation, currentScale);
        break;
    }
    case EditorGizmoOperation::Rotate: {
        const float currentTranslation[3] = {
            transform.TranslateV.x,
            transform.TranslateV.y,
            transform.TranslateV.z
        };
        const float stableRotation[3] = {
            transform.RotateV.x + deltaRotation[0],
            transform.RotateV.y + deltaRotation[1],
            transform.RotateV.z + deltaRotation[2]
        };
        const float currentScale[3] = {
            transform.ScaleV.x < 1.0f ? 1.0f : transform.ScaleV.x,
            transform.ScaleV.y < 1.0f ? 1.0f : transform.ScaleV.y,
            transform.ScaleV.z < 1.0f ? 1.0f : transform.ScaleV.z
        };
        pushTransformEvent(EditorEventType::Rotate, currentTranslation, stableRotation, currentScale);
        break;
    }
    case EditorGizmoOperation::Scale: {
        const float currentTranslation[3] = {
            transform.TranslateV.x,
            transform.TranslateV.y,
            transform.TranslateV.z
        };
        const float currentRotation[3] = {
            transform.RotateV.x,
            transform.RotateV.y,
            transform.RotateV.z
        };
        pushTransformEvent(EditorEventType::Scale, currentTranslation, currentRotation, scale);
        break;
    }
    }
}

void EditorUI::drawPerformanceOverlay() {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowPos(
        ImVec2(io.DisplaySize.x - 10.0f, ImGui::GetFrameHeight() + 10.0f),
        ImGuiCond_Always,
        ImVec2(1.0f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.7f);

    constexpr ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav;

    if (ImGui::Begin("Performance", nullptr, flags)) {
        ImGui::Text("FPS: %.1f", io.Framerate);
        ImGui::Text("Frame time: %.2f ms", 1000.0f / io.Framerate);
    }
    ImGui::End();
}
