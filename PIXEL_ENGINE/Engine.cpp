#include "Engine.h"

#include <filesystem>
using namespace std;

void Engine::visualizeEditorEvents() {

    const auto& SceneInfo = OurSceneSystem->getSceneInfo();
    const auto& EntityList = SceneInfo.EntityList;

    vector<EditorHierarchyView> hierarchyViews;

    // We giving to hierarchy all active entities names and IDs, so we can show them in hierarchy tree
    for (const auto& element : EntityList) {
        const auto& entity = element.second;

        if (!entity.isActive) continue;
        
        hierarchyViews.push_back(EditorHierarchyView{ entity.EntityID, entity.name });
    } 

    OurUISystem->setHierarchyViews(hierarchyViews);

    if (SceneInfo.OurActiveEntityID < 1) {
        OurUISystem->clearSelectedEntityView();
        return;
    }

    // We must give data about selected Entity
    auto it = EntityList.find(SceneInfo.OurActiveEntityID);
    if (it == EntityList.end() || !it->second.isActive) {
        OurUISystem->clearSelectedEntityView();
        return;
    }

    const auto& selectedEntity = it->second;

    EditorEntityView view;

    view.entityID = selectedEntity.EntityID;
    view.meshID = selectedEntity.MeshID;
    view.textureID = selectedEntity.TextureID;
    view.color = selectedEntity.color;
    view.colorEnabled = selectedEntity.isColorActive;
    view.ShaderID = selectedEntity.ShaderID;
    view.name = selectedEntity.name;
    view.position = EditorVec3{
        selectedEntity.TransformInfo.TranslateV.x,
        selectedEntity.TransformInfo.TranslateV.y,
        selectedEntity.TransformInfo.TranslateV.z
    };
    view.rotation = EditorVec3{
        selectedEntity.TransformInfo.RotateV.x,
        selectedEntity.TransformInfo.RotateV.y,
        selectedEntity.TransformInfo.RotateV.z
    };
    view.scale = EditorVec3{
        selectedEntity.TransformInfo.ScaleV.x,
        selectedEntity.TransformInfo.ScaleV.y,
        selectedEntity.TransformInfo.ScaleV.z
    };
    view.worldMatrix = selectedEntity.TransformInfo.OurMatrix;

    OurUISystem->setSelectedEntityView(view);
}

void Engine::processEditorEvents() {

    const vector <EditorEvent> events = EventSystem::getEventsList();

    for (const EditorEvent& event : events) {

        switch (event.type) {

        case EditorEventType::AddObject:
            if (event.info.isEmptyEntity) {
                OurSceneSystem->SceneCreateEntity();
            }
            else {
                OurSceneSystem->SceneCreateEntity(event.info.meshID, event.info.ShaderID, event.info.name);
            }
            continue;

        case EditorEventType::RegisterObject: {
            OurAssetSystem->RegisterFile(event.info.path);
            continue;
        }

        case EditorEventType::ChangeEntityColor:
            OurSceneSystem->ChangeColorEntity(event.info.entityID, event.info.color);
            continue;

        case EditorEventType::ToggleEntityColor:
            if (event.info.colorEnabled) {
                OurSceneSystem->activateColorEntity(event.info.entityID);
            }
            else {
                OurSceneSystem->deactivateColorEntity(event.info.entityID);
            }
            continue;

        case EditorEventType::Translate:
            OurSceneSystem->changeTranslateEntity(event.info.entityID,
                glm::vec3(event.info.position.x, event.info.position.y, event.info.position.z));
            continue;

        case EditorEventType::Rotate:
            OurSceneSystem->changeRotateEntity(event.info.entityID,
                glm::vec3(event.info.rotation.x, event.info.rotation.y, event.info.rotation.z));
            continue;

        case EditorEventType::Scale:
            OurSceneSystem->changeScaleEntity(event.info.entityID,
                glm::vec3(event.info.scale.x, event.info.scale.y, event.info.scale.z));
            continue;

        case EditorEventType::ClearScene:
            m_selectedEntityID = -1;
            OurSceneSystem->SceneClearEntityList();
            Logger::addLog(LOG_INFO, "Scene was cleared successfully\n");
            continue;

        case EditorEventType::ShowAbout:
            Logger::addLog(LOG_INFO, event.info.message);
            continue;

        case EditorEventType::DeleteObject:
            OurSceneSystem->SceneDeleteEntity(event.info.entityID);
            continue;

        case EditorEventType::RenameObject:
            OurSceneSystem->ChangeNameEntity(event.info.entityID, event.info.name);
            continue;

        case EditorEventType::DuplicateObject:
            OurSceneSystem->SceneDuplicateEntity(event.info.entityID);
            continue;

        case EditorEventType::AssignTexture:
            OurSceneSystem->ChangeTextureEntity(event.info.entityID, event.info.textureID);
            continue;

        case EditorEventType::RegisterTexture:
            OurAssetSystem->RegisterFile(event.info.path);
            continue;

        case EditorEventType::CloseWindow:
            glfwSetWindowShouldClose(OurWindow, GLFW_TRUE);
            continue;

        case EditorEventType::GiveEntityInfo:
            m_selectedEntityID = event.info.entityID;
            OurSceneSystem->MakeActiveEntity(m_selectedEntityID);
            continue;

        case EditorEventType::SetGizmoTranslate:
            OurUISystem->setGizmoOperation(EditorGizmoOperation::Translate);
            continue;

        case EditorEventType::SetGizmoRotate:
            OurUISystem->setGizmoOperation(EditorGizmoOperation::Rotate);
            continue;

        case EditorEventType::SetGizmoScale:
            OurUISystem->setGizmoOperation(EditorGizmoOperation::Scale);
            continue;

        case EditorEventType::CameraMoveForward:
            OurSceneSystem->changePosCamera(event.info.entityID, Forward);
            continue;

        case EditorEventType::CameraMoveBackward:
            OurSceneSystem->changePosCamera(event.info.entityID, Back);
            continue;

        case EditorEventType::CameraMouseChange:
            OurSceneSystem->changeAnglesCamera(event.info.cameraID, event.info.xpos, event.info.ypos);
            continue;
        }
    }
}

bool Engine::initailize() {
    glfwInit(); // запуск библиотеки

    // Сейчас будем говорить версию какую хотим(а хотим мы 3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // главная цифра(3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // доп цифра(0.3) 

    // А теперь хотим ли мы поддержку старых версий и функций opengl(нет)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE /*означает что ток современные хотим*/);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    // 2. Получаем текущий видеорежим (разрешение и частоту обновления)
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // 3. Передаем ширину и высоту монитора
    OurWindow = glfwCreateWindow(mode->width, mode->height, "PixelEngine", monitor, NULL);


    if (OurWindow == NULL) {
        cout << "Failed to load a window\n";
        glfwTerminate();
        return false;
    }

    glfwGetFramebufferSize(OurWindow, &screenW, &screenH);
    glfwMakeContextCurrent(OurWindow);
    glfwSwapInterval(1);


    // Чтобы связать новые функции со старыми из 1997 видеокарте необходимо местоположение этих функций 
    // Вручную это трудно так что мы юзаем glad

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to load GLAD\n";
        return false;
    }

    OurRenderSystem = new RenderSystem();
    OurSceneSystem = new SceneSystem(
        static_cast<float>(screenW),
        static_cast<float>(screenH)
    );
    OurAssetSystem = new AssetSystem();

    OurUISystem = new UISystem();
    OurUISystem->init(OurWindow);

    return true;
}

void Engine::run() {

    OurAssetSystem->Run();

    while (!glfwWindowShouldClose(OurWindow)) {

        if (OurUISystem->WindowShouldClose()) {
            return;
        }

        OurInputManager.analyzeInput(OurWindow);
        
        // 1. Очищаем экран общим цветом (фон под UI)
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Добавлен GL_DEPTH_BUFFER_BIT

        // 2. Получаем координаты кармана от UI и выставляем OpenGL Viewport
        int vX, vY, vW, vH;
        OurUISystem->getViewportRect(vX, vY, vW, vH);
        glViewport(vX, vY, vW, vH);

        // Проверяем флаги на события и отображаем их
        processEditorEvents();
        visualizeEditorEvents();
        
        // 3. Рисуем сцену ТОЛЬКО в этом кармане! Твой треугольник будет здесь
        OurRenderSystem->renderScene(OurSceneSystem->getSceneInfo());
        OurSceneSystem->SceneUpdate(static_cast<float>(screenW), static_cast<float>(screenH));

        // 4. Возвращаем Viewport обратно на ВЕСЬ экран, чтобы UI рисовался правильно
        glfwGetFramebufferSize(OurWindow, &screenW, &screenH);
        glViewport(0, 0, screenW, screenH);

        // 5. Отрисовываем сам UI поверх всего
        OurUISystem->beginFrame();
        OurUISystem->render();
        OurUISystem->endFrame();
        //---------------------------------------------------------

        glfwSwapBuffers(OurWindow);    // у нас есть буффер для всех пикселей где какой цвет и вот его надо обновлять
        glfwPollEvents();   // чекает изменения в системе(инпут оутпут и тд)

    }
    return;
}

void Engine::terminate() {
    OurUISystem->shutdown();
    delete OurUISystem;

    glfwTerminate();
    return;
}
