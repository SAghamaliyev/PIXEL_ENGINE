#include "Engine.h"

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
    glfwMakeContextCurrent(OurWindow);


    // Чтобы связать новые функции со старыми из 1997 видеокарте необходимо местоположение этих функций 
    // Вручную это трудно так что мы юзаем glad

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to load GLAD\n";
        return false;
    }

    OurRenderSystem = new RenderSystem();
    OurSceneSystem = new SceneSystem();

    OurEditorUI = new EditorUI();
    OurEditorUI->init(OurWindow, OurSceneSystem);

    return true;
}

void Engine::run() {

    while (!glfwWindowShouldClose(OurWindow)) {

        if (OurEditorUI->WindowShouldClose()) {
            return;
        }

        callSystemInputs(OurWindow);
        
        // 1. Очищаем экран общим цветом (фон под UI)
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);

        // 2. Получаем координаты кармана от UI и выставляем OpenGL Viewport
        int vX, vY, vW, vH;
        OurEditorUI->getViewportRect(vX, vY, vW, vH);
        glViewport(vX, vY, vW, vH);

        // 3. Рисуем сцену ТОЛЬКО в этом кармане! Твой треугольник будет здесь
        OurRenderSystem->renderScene(OurSceneSystem->getSceneInfo());
        OurSceneSystem->SceneUpdate();

        // 4. Возвращаем Viewport обратно на ВЕСЬ экран, чтобы UI рисовался правильно
        int screenW, screenH;
        glfwGetFramebufferSize(OurWindow, &screenW, &screenH);
        glViewport(0, 0, screenW, screenH);

        // 5. Отрисовываем сам UI поверх всего
        OurEditorUI->beginFrame();
        OurEditorUI->render();
        OurEditorUI->endFrame();
        //---------------------------------------------------------

        glfwSwapBuffers(OurWindow);    // у нас есть буффер для всех пикселей где какой цвет и вот его надо обновлять
        glfwPollEvents();   // чекает изменения в системе(инпут оутпут и тд)

    }
    return;
}

void Engine::terminate() {
    OurEditorUI->shutdown();
    delete OurEditorUI;

    glfwTerminate();
    return;
}
