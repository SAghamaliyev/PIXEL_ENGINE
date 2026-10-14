#include "Engine.h"

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Core/Logger/Logger.h"

using namespace std;
using namespace glm;

void Engine::visualizeEditorEvents() {
    const auto& scene_info = m_pSceneSystem->getSceneInfo();
    const auto& entity_list = scene_info.entityList;

    vector<EditorHierarchyView> hierarchy_views;

    // The hierarchy tree needs names and IDs of active entities only.
    for (const auto& element : entity_list) {
        const auto& entity = element.second;

        if (!entity.isActive) {
            continue;
        }

        hierarchy_views.push_back(EditorHierarchyView{ entity.entityID, entity.name });
    }

    m_pUiSystem->setHierarchyViews(hierarchy_views);

    if (scene_info.activeEntityID < 1) {
        m_pUiSystem->clearSelectedEntityView();
        return;
    }

    auto it = entity_list.find(scene_info.activeEntityID);
    if (it == entity_list.end() || !it->second.isActive) {
        m_pUiSystem->clearSelectedEntityView();
        return;
    }

    const auto& selected_entity = it->second;

    EditorEntityView view;

    view.entityID = selected_entity.entityID;
    view.meshID = selected_entity.meshID;
    view.textureID = selected_entity.textureID;
    view.color = selected_entity.color;
    view.colorEnabled = selected_entity.isColorTintEnabled;
    view.shaderID = selected_entity.materialType;
    view.name = selected_entity.name;

    EditorVec3 temp_Translate;

    temp_Translate = { selected_entity.transform.translation.x,
                          selected_entity.transform.translation.y,
                          selected_entity.transform.translation.z};
    view.position = temp_Translate;

    EditorVec3 temp_Rotate;
    temp_Rotate = { selected_entity.transform.rotation.x,
                       selected_entity.transform.rotation.y,
                       selected_entity.transform.rotation.z };
    view.rotation = temp_Rotate;

    EditorVec3 temp_Scale;
    temp_Scale = { selected_entity.transform.scale.x,
                      selected_entity.transform.scale.y,
                      selected_entity.transform.scale.z };
    view.scale = temp_Scale;


    view.worldMatrix = selected_entity.transform.matrix;

    m_pUiSystem->setSelectedEntityView(view);
}

void Engine::processEditorEvents() {
    const vector<EditorEvent> events = EventSystem::getEvents();

    // Mouse movement events arrive every frame, but should rotate the camera
    // only between the "should change" and "should stay" events.
    bool camera_should_change = false;

    for (const EditorEvent& event : events) {
        switch (event.type) {
            case EditorEventType::AddEntity:
                if (event.info.isEmptyEntity) {
                    m_pSceneSystem->sceneCreateEntity();
                }
                else {
                    m_pSceneSystem->sceneCreateEntity(event.info.meshID,
                        event.info.materialType,
                        event.info.name);
                }
                break;

            case EditorEventType::RegisterMesh:
            case EditorEventType::RegisterTexture:
                m_pAssetSystem->registerFile(event.info.path);
                break;

            case EditorEventType::ChangeEntityColor:
                m_pSceneSystem->changeColorEntity(event.info.entityID, event.info.color);
                break;

            case EditorEventType::ToggleEntityColor:
                if (event.info.isColorTintEnabled) {
                    m_pSceneSystem->enableColorTintEntity(event.info.entityID);
                }
                else {
                    m_pSceneSystem->disableColorTintEntity(event.info.entityID);
                }
                break;

            case EditorEventType::Translate: {
                const EditorVec3 temp = event.info.position;
                m_pSceneSystem->changeTranslateEntity(event.info.entityID,
                                                      vec3(temp.x, temp.y, temp.z));
                break;
            }

            case EditorEventType::Rotate: {
                const EditorVec3 temp = event.info.rotation;
                m_pSceneSystem->changeRotateEntity(event.info.entityID,
                                                   vec3(temp.x, temp.y, temp.z));
                break;
            }

            case EditorEventType::Scale: {
                const EditorVec3 temp = event.info.scale;
                m_pSceneSystem->changeScaleEntity(event.info.entityID,
                                                  vec3(temp.x, temp.y, temp.z));
                break;
            }

            case EditorEventType::ClearScene:
                m_pSceneSystem->sceneClearEntities();
                Logger::addLog(LogType::LOG_INFO, "Scene was cleared successfully\n");
                break;

            case EditorEventType::ShowAbout:
                Logger::addLog(LogType::LOG_INFO, event.info.message);
                break;

            case EditorEventType::DeleteEntity:
                m_pSceneSystem->sceneDeleteEntity(event.info.entityID);
                break;

            case EditorEventType::RenameEntity:
                m_pSceneSystem->changeNameEntity(event.info.entityID, event.info.name);
                break;

            case EditorEventType::DuplicateEntity:
                m_pSceneSystem->sceneDuplicateEntity(event.info.entityID);
                break;

            case EditorEventType::AssignTexture:
                m_pSceneSystem->changeTextureEntity(event.info.entityID,
                    event.info.textureID);
                break;

            case EditorEventType::CloseWindow:
                glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE);
                break;

            case EditorEventType::RequestEntityInfo:
                m_pSceneSystem->sceneSelectEntity(event.info.entityID);
                break;

            case EditorEventType::SetGizmoTranslate:
                m_pUiSystem->setGizmoOperation(GizmoOperation::Translate);
                break;

            case EditorEventType::SetGizmoRotate:
                m_pUiSystem->setGizmoOperation(GizmoOperation::Rotate);
                break;

            case EditorEventType::SetGizmoScale:
                m_pUiSystem->setGizmoOperation(GizmoOperation::Scale);
                break;

            case EditorEventType::CameraMoveForward:
                m_pSceneSystem->changePositionCamera(event.info.entityID, Movement::Forward);
                break;

            case EditorEventType::CameraMoveBackward:
                m_pSceneSystem->changePositionCamera(event.info.entityID, Movement::Back);
                break;

            case EditorEventType::CameraMoveLeft:
                m_pSceneSystem->changePositionCamera(event.info.entityID, Movement::Left);
                break;

            case EditorEventType::CameraMoveRight:
                m_pSceneSystem->changePositionCamera(event.info.entityID, Movement::Right);
                break;

            case EditorEventType::CameraMoveUp:
                m_pSceneSystem->changePositionCamera(event.info.entityID, Movement::Up);
                break;

            case EditorEventType::CameraMoveDown:
                m_pSceneSystem->changePositionCamera(event.info.entityID, Movement::Down);
                break;

            case EditorEventType::UpdateCameraRotation:
                if (camera_should_change) {
                    m_pSceneSystem->changeRotationCamera(event.info.cameraID,
                        event.info.mouseOffsetX,
                        event.info.mouseOffsetY);
                }
                break;

            case EditorEventType::SetCameraMouseEnabled:
                if (camera_should_change) {
                    camera_should_change = false;
                }
                else {
                    camera_should_change = true;
                }
                break;
            case EditorEventType::SetEntityMaterial:
                break;
            case EditorEventType::OpenScene:
                break;
            case EditorEventType::SaveScene:
                break;
            case EditorEventType::SaveSceneAs:
                break;
            case EditorEventType::SubmitConsoleCommand:
                break;
            case EditorEventType::ClearConsole:
                break;
        }
    }
}

bool Engine::initialize() {
    if (!glfwInit()) {
        cout << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* p_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* p_video_mode = glfwGetVideoMode(p_monitor);

    // Passing the monitor makes the window fullscreen at the native resolution.
    m_pWindow = glfwCreateWindow(p_video_mode->width,
        p_video_mode->height,
        "PixelEngine",
        p_monitor,
        nullptr);

    if (m_pWindow == nullptr) {
        cout << "Failed to create a window\n";
        glfwTerminate();
        return false;
    }

    glfwGetFramebufferSize(m_pWindow, &m_viewportWidth, &m_viewportHeight);
    glfwMakeContextCurrent(m_pWindow);
    glfwSwapInterval(1);

    // OpenGL function addresses are resolved at runtime by the driver,
    // glad loads them so we don't have to do it by hand.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to load GLAD\n";
        glfwTerminate();
        return false;
    }

    // SceneSystem needs the window size at construction, so it can't exist
    // before the window does.
    m_pSceneSystem = new SceneSystem(m_viewportWidth, m_viewportHeight);

    m_pAssetSystem = new AssetSystem();
    m_pRenderSystem = new RenderSystem();
    m_pInputManager = new InputManager();
    m_pUiSystem = new UISystem();

    m_pUiSystem->init(m_pWindow);

    return true;
}

void Engine::run() {
    m_pAssetSystem->run();

    while (!glfwWindowShouldClose(m_pWindow)) {
        if (m_pUiSystem->windowShouldClose()) {
            break;
        }

        m_pInputManager->processInput(m_pWindow);

        // The background is visible behind the UI panels.
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // The UI decides which part of the window is left for the scene,
        // so the scene must be rendered only inside that rectangle.
        int viewport_x = 0;
        int viewport_y = 0;
        int viewport_width = 0;
        int viewport_height = 0;
        m_pUiSystem->getViewportRect(viewport_x, viewport_y, viewport_width, viewport_height);
        glViewport(viewport_x, viewport_y, viewport_width, viewport_height);

        processEditorEvents();
        visualizeEditorEvents();

        m_pRenderSystem->renderScene(m_pSceneSystem->getSceneInfo());
        m_pSceneSystem->sceneUpdate(m_viewportWidth,
            m_viewportHeight);

        // The UI is drawn over the whole window, so the viewport has to be restored.
        glfwGetFramebufferSize(m_pWindow, &m_viewportWidth, &m_viewportHeight);
        glViewport(0, 0, m_viewportWidth, m_viewportHeight);

        m_pUiSystem->beginFrame();
        m_pUiSystem->render();
        m_pUiSystem->endFrame();

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }
}

void Engine::terminate() {
    m_pUiSystem->shutdown();

    delete m_pSceneSystem;
    m_pSceneSystem = nullptr;

    glfwTerminate();
}