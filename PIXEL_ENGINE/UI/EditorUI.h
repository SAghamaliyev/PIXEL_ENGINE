#pragma once

// ===== Dear ImGui =====
#include "Materials/imgui.h"
#include "Materials/Backends/imgui_impl_glfw.h"
#include "Materials/Backends/imgui_impl_opengl3.h"

// ===== Std =====
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "../SceneSystem/SceneSystem.h"

// ==========================================================================
// EditorUI — класс редакторского интерфейса на Dear ImGui.
//
// ВАЖНО: Этот класс содержит ТОЛЬКО UI-отрисовку. Вся логика (создание
// объектов, загрузка файлов и т.д.) должна быть добавлена тобой.
// Ищи комментарии "// TODO:" — там описано, что каждый элемент должен делать.
//
// Чтобы подключить: создай экземпляр EditorUI в Engine, вызови init() после
// создания окна, beginFrame()/render()/endFrame() в игровом цикле, shutdown()
// при выходе.
// ==========================================================================

class EditorUI {
private:
    GLFWwindow* m_Window = nullptr;
    SceneSystem* m_SceneSystem = nullptr;

    // ===================== Общее состояние =====================
    int m_SelectedEntityID = -1;   // ID выбранного entity (-1 = ничего)

    // Видимость панелей (переключается через меню View)
    bool m_ShowHierarchy     = true;
    bool m_ShowInspector     = true;
    bool m_ShowContentBrowser = true;
    bool m_ShowConsole       = true;

    // ===================== Hierarchy State =====================
    char m_RenameBuffer[128]  = "";
    bool m_RenamingEntity     = false;
    int  m_RenamingEntityID   = -1;
    bool m_OpenRenamePopup    = false;

    // ===================== Inspector State =====================
    char  m_EntityNameBuffer[128] = "";
    float m_Position[3]  = { 0.0f, 0.0f, 0.0f };
    float m_Rotation[3]  = { 0.0f, 0.0f, 0.0f };
    float m_Scale[3]     = { 1.0f, 1.0f, 1.0f };
    int   m_CurrentMaterial = 0;

    // ===================== Content Browser State =====================
    char        m_SearchBuffer[256] = "";
    std::string m_CurrentDirectory  = "src";
    std::string m_SelectedFilePath  = "";

    // ===================== Console State =====================
    struct LogEntry {
        enum Type { LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_DEBUG };
        Type        type;
        std::string message;
    };
    std::vector<LogEntry> m_ConsoleLogs;
    char m_CommandBuffer[256] = "";
    bool m_FilterInfo    = true;
    bool m_FilterWarning = true;
    bool m_FilterError   = true;
    bool m_FilterDebug   = true;
    bool m_ScrollToBottom = true;

    // ===================== Приватные методы отрисовки =====================
    void drawMainMenuBar();
    void drawSceneHierarchy();
    void drawInspector();
    void drawContentBrowser();
    void drawConsole();

    // Вспомогательная — задаёт начальные позиции/размеры окон
    void setupInitialLayout();

    // ===================== Themes =====================
    void setThemeDark();
    void setThemeBlue();
    void setThemePurple();

public:
    EditorUI()  = default;
    ~EditorUI() = default;

    // ===================== Lifecycle =====================
    void init(GLFWwindow* window, SceneSystem* sceneSystem);     // Вызови после glfwCreateWindow + gladLoad
    void beginFrame();                 // Вызови перед render() каждый кадр
    void render();                     // Рисует все панели
    void endFrame();                   // Вызови после render() каждый кадр
    void shutdown();                   // Вызови перед glfwTerminate()

    // ===================== Console API =====================
    void addLog(LogEntry::Type type, const std::string& message);
    void clearLogs();

    // ===================== Getters =====================
    int getSelectedEntityID() const { return m_SelectedEntityID; }
    
    // Получить координаты пустого "кармана" в центре для рендера сцены
    void getViewportRect(int& outX, int& outY, int& outW, int& outH);
};
