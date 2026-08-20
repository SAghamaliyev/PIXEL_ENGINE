#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include "Input/SystemInputs.h"
#include "RenderSystem/RenderSystem.h"

using namespace std;

void runEditMode();


int main(int argc, char** argv) {
    bool isEditorMode = true;

    for (int i = 0; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--game" || arg == "-g") {
            isEditorMode = false;
        }
    }

    if (isEditorMode)
        runEditMode();

    return 0;
}

void runEditMode() {
    bool isEditMode = true;
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
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "PixelEngine", monitor, NULL);


    if (window == NULL) {
        cout << "Failed to load a window\n";
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);


    // Чтобы связать новые функции со старыми из 1997 видеокарте необходимо местоположение этих функций 
    // Вручную это трудно так что мы юзаем glad

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to load GLAD\n";
        return;
    }


    // Отрисовка рендеринг крч
    glViewport(0, 0, 800, 600); // первые 2 параметра это координаты(0 и 0 это опенгл рисует с нижнего левого поля на 800 и 600 пх



    while (!glfwWindowShouldClose(window)) {
        if (isEditMode)
            callSystemInputs(window);
        //-----------------------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Запоминает этот цвет, чтобы после очищение ею залить
        glClear(GL_COLOR_BUFFER_BIT);   // Очищает весь буффер

        RenderSystem OurRender;
        OurRender.renderScene();

        //-----------------------------------
        glfwSwapBuffers(window);    // у нас есть буффер для всех пикселей где какой цвет и вот его надо обновлять
        glfwPollEvents();   // чекает изменения в системе(инпут оутпут и тд)

    }

    glfwTerminate();
    return;
}
//
//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"
//#include <GLFW/glfw3.h> // Ваш оконный менеджер
//
//int main() {
//    // 1. Инициализация GLFW и создание окна (стандартный код OpenGL)
//    glfwInit();
//    // 1. Получаем главный монитор
//    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
//
//    // 2. Получаем текущий видеорежим (разрешение и частоту обновления)
//    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
//
//    // 3. Передаем ширину и высоту монитора
//    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "ImGui Example", monitor, NULL);
//    glfwMakeContextCurrent(window);
//
//    // 2. Инициализация контекста ImGui
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    ImGui::StyleColorsDark(); // Установка темной темы
//
//    // 3. Инициализация бэкендов
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init("#version 130");
//
//    // 4. Главный цикл приложения
//    while (!glfwWindowShouldClose(window)) {
//        glfwPollEvents();
//
//        // Начало нового кадра ImGui
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        // ==========================================
//        // ЗДЕСЬ ВЫ ПИШЕТЕ КОД ВАШЕГО ИНТЕРФЕЙСА
//
//        ImGui::Begin("Inspector");
//
//        
//        ImGui::End();
//        // ==========================================
//
//        // Рендеринг ImGui
//        ImGui::Render();
//        int display_w, display_h;
//        glfwGetFramebufferSize(window, &display_w, &display_h);
//        glViewport(0, 0, display_w, display_h);
//        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        glfwSwapBuffers(window);
//    }
//
//    // 5. Очистка памяти при выходе
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//
//    glfwDestroyWindow(window);
//    glfwTerminate();
//
//    return 0;
//}