#include "EditorUI.h"
#include <cstring>   // strcpy, strncpy
#include <algorithm> // std::remove_if
#include <filesystem>

// ==========================================================================
//  LIFECYCLE
// ==========================================================================

void EditorUI::init(GLFWwindow* window, SceneSystem* sceneSystem) {
    m_Window = window;
    m_SceneSystem = sceneSystem;

    // Создаём ImGui контекст
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Навигация клавиатурой

    // Включаем встроенный пиксельный шрифт (ProggyClean) и увеличиваем его
    io.Fonts->AddFontDefault();
    io.FontGlobalScale = 1.3f; // Слегка увеличиваем, чтобы был приятным и читаемым

    // Устанавливаем дефолтную темную тему
    setThemeDark();

    // Инициализация бэкендов
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // TODO: Добавляй логи через addLog() из движка, например:
    //       addLog(LogEntry::LOG_INFO, "Engine initialized.");
}

void EditorUI::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void EditorUI::render() {
    setupInitialLayout();
    drawMainMenuBar();

    if (m_ShowHierarchy)      drawSceneHierarchy();
    if (m_ShowInspector)       drawInspector();
    if (m_ShowContentBrowser)  drawContentBrowser();
    if (m_ShowConsole)         drawConsole();
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

// ==========================================================================
//  THEMES
// ==========================================================================

void EditorUI::setThemeDark() {
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding    = 0.0f;
    style.FrameRounding     = 2.0f;
    style.ScrollbarRounding = 2.0f;
    style.WindowBorderSize  = 1.0f;
    style.FramePadding      = ImVec2(6, 4);
    style.ItemSpacing       = ImVec2(8, 4);
}

void EditorUI::setThemeBlue() {
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Base colors for Blue Theme
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg]       = ImVec4(0.05f, 0.1f, 0.2f, 1.0f);
    colors[ImGuiCol_Header]         = ImVec4(0.15f, 0.3f, 0.5f, 1.0f);
    colors[ImGuiCol_HeaderHovered]  = ImVec4(0.2f, 0.4f, 0.6f, 1.0f);
    colors[ImGuiCol_HeaderActive]   = ImVec4(0.25f, 0.45f, 0.65f, 1.0f);
    colors[ImGuiCol_Button]         = ImVec4(0.15f, 0.3f, 0.5f, 1.0f);
    colors[ImGuiCol_ButtonHovered]  = ImVec4(0.2f, 0.4f, 0.6f, 1.0f);
    colors[ImGuiCol_ButtonActive]   = ImVec4(0.25f, 0.45f, 0.65f, 1.0f);
    colors[ImGuiCol_FrameBg]        = ImVec4(0.1f, 0.15f, 0.25f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.25f, 0.35f, 1.0f);
    colors[ImGuiCol_FrameBgActive]  = ImVec4(0.2f, 0.3f, 0.4f, 1.0f);
    colors[ImGuiCol_TitleBg]        = ImVec4(0.08f, 0.12f, 0.2f, 1.0f);
    colors[ImGuiCol_TitleBgActive]  = ImVec4(0.12f, 0.18f, 0.28f, 1.0f);
}

void EditorUI::setThemePurple() {
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Base colors for Purple/Pink Theme
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg]       = ImVec4(0.12f, 0.08f, 0.15f, 1.0f);
    colors[ImGuiCol_Header]         = ImVec4(0.4f, 0.2f, 0.5f, 1.0f);
    colors[ImGuiCol_HeaderHovered]  = ImVec4(0.5f, 0.25f, 0.6f, 1.0f);
    colors[ImGuiCol_HeaderActive]   = ImVec4(0.6f, 0.3f, 0.7f, 1.0f);
    colors[ImGuiCol_Button]         = ImVec4(0.4f, 0.2f, 0.5f, 1.0f);
    colors[ImGuiCol_ButtonHovered]  = ImVec4(0.5f, 0.25f, 0.6f, 1.0f);
    colors[ImGuiCol_ButtonActive]   = ImVec4(0.6f, 0.3f, 0.7f, 1.0f);
    colors[ImGuiCol_FrameBg]        = ImVec4(0.2f, 0.1f, 0.25f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.15f, 0.35f, 1.0f);
    colors[ImGuiCol_FrameBgActive]  = ImVec4(0.4f, 0.2f, 0.45f, 1.0f);
    colors[ImGuiCol_TitleBg]        = ImVec4(0.1f, 0.05f, 0.12f, 1.0f);
    colors[ImGuiCol_TitleBgActive]  = ImVec4(0.25f, 0.1f, 0.3f, 1.0f);
}

// ==========================================================================
//  LAYOUT — начальная расстановка окон (ImGuiCond_FirstUseEver)
// ==========================================================================

void EditorUI::setupInitialLayout() {
    int screenW, screenH;
    glfwGetFramebufferSize(m_Window, &screenW, &screenH);

    float menuBarH = ImGui::GetFrameHeight();       // высота меню-бара
    float totalH   = (float)screenH - menuBarH;

    // Пропорции (подогнаны под референс)
    float leftW    = (float)screenW * 0.18f;         // Scene Hierarchy ширина
    float rightW   = (float)screenW * 0.22f;         // Inspector ширина
    float topH     = totalH * 0.62f;                 // Верхняя зона (hierarchy + viewport)
    float bottomH  = totalH * 0.38f;                 // Нижняя зона (content browser + console)
    float centerW  = (float)screenW - leftW - rightW;
    float halfBottomW = ((float)screenW - rightW) * 0.5f;

    // Scene Hierarchy — верхний левый угол
    ImGui::SetNextWindowPos(ImVec2(0, menuBarH), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(leftW, topH), ImGuiCond_FirstUseEver);

    // Inspector — правая сторона, на всю высоту
    // (позиция задаётся в drawInspector через SetNextWindow перед Begin)

    // Content Browser — нижняя левая
    // Console — нижняя правая
    // Позиции задаются в соответствующих draw-методах
    // Сохраняем значения в статических переменных для доступа из draw-методов

    // Используем static для передачи в draw-функции
    static bool layoutSet = false;
    if (!layoutSet) {
        layoutSet = true;

        // Эти значения будут использоваться один раз при первом кадре
        // Далее пользователь может свободно перемещать окна
    }

    // Каждый draw-метод сам выставляет SetNextWindowPos/Size с ImGuiCond_FirstUseEver
    // Мы передаём данные через прямой вызов перед каждым Begin()
    // Поэтому ниже — helper static vars, чтобы draw-методы к ним обращались.
    // (Можно было бы сделать членами класса, но они нужны один раз)

    // -- Храним как static, чтобы draw* методы могли их прочитать --
    // Они уже установлены в каждом draw*() методе напрямую.
}

// ==========================================================================
//  MAIN MENU BAR
// ==========================================================================

void EditorUI::drawMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {

        // ======================== FILE ========================
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
                // TODO: Создать новую пустую сцену, очистить все entity
            }
            if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
                // TODO: Открыть диалог выбора файла сцены и загрузить её
            }
            if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
                // TODO: Сохранить текущую сцену в файл
            }
            if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) {
                // TODO: Сохранить сцену с выбором нового имени/пути
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
                // TODO: Закрыть приложение (glfwSetWindowShouldClose)
            }
            ImGui::EndMenu();
        }

        // ======================== EDIT ========================
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
                // TODO: Отменить последнее действие
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
                // TODO: Повторить отменённое действие
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Preferences")) {
                // TODO: Открыть окно настроек редактора
            }
            ImGui::EndMenu();
        }

        // ======================== VIEW ========================
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Scene Hierarchy",  nullptr, &m_ShowHierarchy);
            ImGui::MenuItem("Inspector",        nullptr, &m_ShowInspector);
            ImGui::MenuItem("Content Browser",  nullptr, &m_ShowContentBrowser);
            ImGui::MenuItem("Console",          nullptr, &m_ShowConsole);
            ImGui::EndMenu();
        }
        
        // ======================== THEME ========================
        if (ImGui::BeginMenu("Theme")) {
            if (ImGui::MenuItem("Dark Theme"))   { setThemeDark(); }
            if (ImGui::MenuItem("Blue Theme"))   { setThemeBlue(); }
            if (ImGui::MenuItem("Purple/Pink Theme")) { setThemePurple(); }
            ImGui::EndMenu();
        }

        // ======================== HELP ========================
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) {
                // TODO: Показать информационное окно About (версия, авторы)
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

// ==========================================================================
//  SCENE HIERARCHY — список объектов на сцене
// ==========================================================================

void EditorUI::drawSceneHierarchy() {
    int screenW, screenH;
    glfwGetFramebufferSize(m_Window, &screenW, &screenH);
    float menuBarH = ImGui::GetFrameHeight();
    float totalH   = (float)screenH - menuBarH;
    float leftW    = (float)screenW * 0.18f;
    float topH     = totalH * 0.62f;

    ImGui::SetNextWindowPos(ImVec2(0, menuBarH), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(leftW, topH), ImGuiCond_Always);

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize 
                                 | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin("Scene Hierarchy", nullptr, windowFlags);

    // --- Кнопка добавления нового entity ---
    if (ImGui::Button("Add Entity")) {
        // Создаем пустой/дефолтный объект
        if (m_SceneSystem) {
            m_SceneSystem->SceneCreateEntity("src/Objects/Triangle.obj", Default, "NewObject");
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete")) {
        if (m_SceneSystem && m_SelectedEntityID >= 0) {
            m_SceneSystem->SceneDeleteEntity(m_SelectedEntityID);
            m_SelectedEntityID = -1;
        }
    }

    //---------------------------------------------------------
    // ЛОГИКА ИЕРАРХИИ (Связь с SceneSystem)
    //---------------------------------------------------------
    // --- Список entity ---
    if (m_SceneSystem) {
        const auto& entityList = m_SceneSystem->getSceneInfo().EntityList;
        if (entityList.empty()) {
            ImGui::TextDisabled("(empty - add entities via Add Entity or Content Browser)");
        } else {
            for (const auto& pair : entityList) {
                const EntityUnit& entity = pair.second;
                unsigned int id = entity.EntityID;
                bool isSelected = (m_SelectedEntityID == (int)id);
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
                                         | ImGuiTreeNodeFlags_SpanAvailWidth;
                if (isSelected) flags |= ImGuiTreeNodeFlags_Selected;
    
                bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)id, flags, "%s", entity.name.c_str());
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
                    m_SelectedEntityID = id;
                }
    
                // ПКМ контекст-меню
                if (ImGui::BeginPopupContextItem()) {
                    if (ImGui::MenuItem("Rename"))    { 
                        m_OpenRenamePopup = true; 
                        m_RenamingEntityID = id; 
                        strncpy_s(m_RenameBuffer, sizeof(m_RenameBuffer), entity.name.c_str(), _TRUNCATE);
                    }
                    if (ImGui::MenuItem("Delete"))    { 
                        m_SceneSystem->SceneDeleteEntity(id); 
                        if (m_SelectedEntityID == (int)id) m_SelectedEntityID = -1;
                    }
                    if (ImGui::MenuItem("Duplicate")) { 
                        m_SceneSystem->SceneCreateEntity(entity.path, entity.MaterialID, entity.name + "_copy");
                    }
                    ImGui::EndPopup();
                }
    
                if (nodeOpen) ImGui::TreePop();
            }
        }
    }
    //---------------------------------------------------------

    // --- Popup для переименования ---
    if (m_OpenRenamePopup) {
        ImGui::OpenPopup("Rename Entity");
        m_OpenRenamePopup = false;
    }
    if (ImGui::BeginPopupModal("Rename Entity", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("New name:");
        ImGui::InputText("##rename", m_RenameBuffer, sizeof(m_RenameBuffer));

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            // TODO: Применить новое имя к entity
            //       EntityUnit& entity = SceneSystem->SceneGetEntity(m_RenamingEntityID);
            //       entity.name = std::string(m_RenameBuffer);
            m_RenamingEntity = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            m_RenamingEntity = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

// ==========================================================================
//  INSPECTOR — свойства выбранного entity
// ==========================================================================

void EditorUI::drawInspector() {
    int screenW, screenH;
    glfwGetFramebufferSize(m_Window, &screenW, &screenH);
    float menuBarH = ImGui::GetFrameHeight();
    float totalH   = (float)screenH - menuBarH;
    float rightW   = (float)screenW * 0.22f;

    ImGui::SetNextWindowPos(ImVec2((float)screenW - rightW, menuBarH), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(rightW, totalH), ImGuiCond_Always);

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize 
                                 | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin("Inspector", nullptr, windowFlags);

    // Если ничего не выбрано
    if (m_SelectedEntityID < 0) {
        ImGui::TextDisabled("No entity selected");
        ImGui::End();
        return;
    }

    // ======================== HEADER — Имя entity ========================
    //---------------------------------------------------------
    // ЛОГИКА ИНСПЕКТОРА (Синхронизация с выбранным объектом)
    //---------------------------------------------------------
    EntityUnit& entity = m_SceneSystem->SceneGetEntity(m_SelectedEntityID);
    
    // При смене выбора обновляем буфер
    static int lastSelectedID = -1;
    if (lastSelectedID != m_SelectedEntityID) {
        strncpy_s(m_EntityNameBuffer, sizeof(m_EntityNameBuffer), entity.name.c_str(), _TRUNCATE);
        lastSelectedID = m_SelectedEntityID;
    }

    if (ImGui::InputText("##EntityName", m_EntityNameBuffer, sizeof(m_EntityNameBuffer))) {
        // Заглушка: тут нужен сеттер в EntityManager, но в твоем коде его нет
        // entity.name = m_EntityNameBuffer; 
    }
    //---------------------------------------------------------

    // ======================== ID (read-only) ========================
    ImGui::Text("Entity ID: %d", m_SelectedEntityID);

    ImGui::Separator();

    // ======================== TRANSFORM ========================
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::DragFloat3("Position", m_Position, 0.01f);
        // TODO: Привязать Position к реальной позиции entity (когда добавишь Transform)
        //       entity.transform.position = glm::vec3(m_Position[0], m_Position[1], m_Position[2]);

        ImGui::DragFloat3("Rotation", m_Rotation, 0.1f);
        // TODO: Привязать Rotation к реальному повороту entity

        ImGui::DragFloat3("Scale", m_Scale, 0.01f);
        // TODO: Привязать Scale к реальному масштабу entity
    }

    // ======================== MESH RENDERER ========================
    if (ImGui::CollapsingHeader("MeshRenderer", ImGuiTreeNodeFlags_DefaultOpen)) {
        // --- Mesh / Model Path ---
        char meshPath[256];
        strncpy_s(meshPath, sizeof(meshPath), entity.path.c_str(), _TRUNCATE);
        ImGui::InputText("Model", meshPath, sizeof(meshPath), ImGuiInputTextFlags_ReadOnly);
        // TODO: Кнопка "..." рядом для выбора другого меша через Content Browser

        ImGui::Separator();

        // --- Material Type (dropdown) ---
        const char* materialNames[] = {
            "Default", "Unlit", "PBR_Standard", "BlinnPhong", 
            "Glass", "Water", "Terrain", "Skybox", "Custom"
        };
        m_CurrentMaterial = (int)entity.MaterialID;
        if (ImGui::Combo("Material", &m_CurrentMaterial, materialNames, IM_ARRAYSIZE(materialNames))) {
            // Заглушка: тут нужен сеттер в EntityManager
            // entity.MaterialID = (MaterialType)m_CurrentMaterial;
        }
    }

    // ======================== SHADER INFO ========================
    if (ImGui::CollapsingHeader("Shader", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Shader Program ID: ---");
        ImGui::TextDisabled("(determined by material type)");
    }

    ImGui::Separator();

    // ======================== ADD COMPONENT ========================
    if (ImGui::Button("Add Component", ImVec2(-1, 0))) {
        ImGui::OpenPopup("AddComponentPopup");
    }
    if (ImGui::BeginPopup("AddComponentPopup")) {
        // TODO: Список доступных компонентов для добавления
        if (ImGui::MenuItem("RigidBody")) {
            // TODO: Добавить компонент RigidBody к entity
        }
        if (ImGui::MenuItem("Script")) {
            // TODO: Добавить компонент Script к entity
        }
        if (ImGui::MenuItem("Collider")) {
            // TODO: Добавить компонент Collider к entity
        }
        if (ImGui::MenuItem("Light")) {
            // TODO: Добавить компонент Light к entity
        }
        if (ImGui::MenuItem("Camera")) {
            // TODO: Добавить компонент Camera к entity
        }
        if (ImGui::MenuItem("Audio Source")) {
            // TODO: Добавить компонент AudioSource к entity
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

// ==========================================================================
//  CONTENT BROWSER — файловый браузер ассетов
// ==========================================================================

void EditorUI::drawContentBrowser() {
    int screenW, screenH;
    glfwGetFramebufferSize(m_Window, &screenW, &screenH);
    float menuBarH  = ImGui::GetFrameHeight();
    float totalH    = (float)screenH - menuBarH;
    float topH      = totalH * 0.62f;
    float bottomH   = totalH * 0.38f;
    float rightW    = (float)screenW * 0.22f;
    float halfW     = ((float)screenW - rightW) * 0.5f;

    ImGui::SetNextWindowPos(ImVec2(0, menuBarH + topH), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(halfW, bottomH), ImGuiCond_Always);

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize 
                                 | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin("Content Browser", nullptr, windowFlags);

    // ======================== ВЕРХНЯЯ ПАНЕЛЬ ========================
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
    ImGui::InputTextWithHint("##Search", "Search...", m_SearchBuffer, sizeof(m_SearchBuffer));
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        m_CurrentDirectory = "src";
        m_SelectedFilePath = "";
    }
    ImGui::SameLine();
    if (ImGui::Button("Up")) {
        std::filesystem::path current(m_CurrentDirectory);
        if (current.has_parent_path()) {
            m_CurrentDirectory = current.parent_path().string();
            m_SelectedFilePath = "";
        }
    }

    ImGui::Separator();

    // ======================== ОСНОВНАЯ ОБЛАСТЬ ========================
    // Разделяем на 2 колонки: дерево папок слева, файлы справа
    ImGui::Columns(2, "ContentBrowserColumns", true);
    ImGui::SetColumnWidth(0, 150.0f);

    // --------- ЛЕВАЯ КОЛОНКА: Дерево папок ---------
    ImGui::BeginChild("FolderTree", ImVec2(0, 0), false);

    //---------------------------------------------------------
    // ЛОГИКА ДЕРЕВА ПАПОК (std::filesystem)
    //---------------------------------------------------------
    if (ImGui::TreeNodeEx("src", ImGuiTreeNodeFlags_DefaultOpen)) {
        try {
            for (const auto& entry : std::filesystem::directory_iterator("src")) {
                if (entry.is_directory()) {
                    if (ImGui::Selectable(entry.path().filename().string().c_str(), m_CurrentDirectory == entry.path().string())) {
                        m_CurrentDirectory = entry.path().string();
                        m_SelectedFilePath = "";
                    }
                }
            }
        } catch (...) {}
        ImGui::TreePop();
    }
    //---------------------------------------------------------

    ImGui::EndChild();

    // --------- ПРАВАЯ КОЛОНКА: Файлы и иконки ---------
    ImGui::NextColumn();
    ImGui::BeginChild("FileView", ImVec2(0, 0), false);

    // Текущий путь
    ImGui::TextDisabled("Path: %s", m_CurrentDirectory.c_str());
    ImGui::Separator();

    //---------------------------------------------------------
    // ЛОГИКА ФАЙЛОВ И ДОБАВЛЕНИЯ НА СЦЕНУ
    //---------------------------------------------------------
    float buttonSize = 72.0f;
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columns = (int)(panelWidth / (buttonSize + 8.0f));
    if (columns < 1) columns = 1;
    
    int i = 0;
    try {
        for (const auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
            if (entry.is_directory()) continue;
            
            std::string filename = entry.path().filename().string();
            
            if (m_SearchBuffer[0] != '\0') {
                if (filename.find(m_SearchBuffer) == std::string::npos) continue;
            }
            
            ImGui::PushID(i++);
            
            bool isSelected = (m_SelectedFilePath == entry.path().string());
            if (isSelected) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.8f, 1.0f));

            if (ImGui::Button(filename.c_str(), ImVec2(buttonSize, buttonSize))) {
                m_SelectedFilePath = entry.path().string();
            }

            if (isSelected) ImGui::PopStyleColor();

            // ДВОЙНОЙ КЛИК -> Добавление на сцену
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                if (m_SceneSystem && (entry.path().extension() == ".obj" || entry.path().extension() == ".fbx")) {
                    std::string pathString = entry.path().string();
                    std::replace(pathString.begin(), pathString.end(), '\\', '/');
                    m_SceneSystem->SceneCreateEntity(pathString, Default, entry.path().stem().string());
                    addLog(LogEntry::LOG_INFO, "Added " + filename + " to scene.");
                }
            }
            
            // ПКМ МЕНЮ -> Добавление на сцену
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Add to Scene")) {
                    if (m_SceneSystem && (entry.path().extension() == ".obj" || entry.path().extension() == ".fbx")) {
                        std::string pathString = entry.path().string();
                        std::replace(pathString.begin(), pathString.end(), '\\', '/');
                        m_SceneSystem->SceneCreateEntity(pathString, Default, entry.path().stem().string());
                        addLog(LogEntry::LOG_INFO, "Added " + filename + " to scene.");
                    }
                }
                ImGui::EndPopup();
            }

            if (i % columns != 0) ImGui::SameLine();
            ImGui::PopID();
        }
    } catch (...) {
        ImGui::TextColored(ImVec4(1,0,0,1), "Error reading directory");
    }
    //---------------------------------------------------------

    ImGui::EndChild();

    ImGui::Columns(1); // Сброс колонок

    ImGui::End();
}

// ==========================================================================
//  CONSOLE — терминал с цветными логами
// ==========================================================================

void EditorUI::drawConsole() {
    int screenW, screenH;
    glfwGetFramebufferSize(m_Window, &screenW, &screenH);
    float menuBarH  = ImGui::GetFrameHeight();
    float totalH    = (float)screenH - menuBarH;
    float topH      = totalH * 0.62f;
    float bottomH   = totalH * 0.38f;
    float rightW    = (float)screenW * 0.22f;
    float halfW     = ((float)screenW - rightW) * 0.5f;

    ImGui::SetNextWindowPos(ImVec2(halfW, menuBarH + topH), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2((float)screenW - halfW, bottomH), ImGuiCond_Always);

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize 
                                 | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin("Console", nullptr, windowFlags);

    // ======================== ВЕРХНЯЯ ПАНЕЛЬ — фильтры + очистка ========================
    if (ImGui::Button("Clear")) {
        // Очистка всех логов
        clearLogs();
    }

    ImGui::SameLine();
    ImGui::Checkbox("Info", &m_FilterInfo);
    ImGui::SameLine();
    ImGui::Checkbox("Warning", &m_FilterWarning);
    ImGui::SameLine();
    ImGui::Checkbox("Error", &m_FilterError);
    ImGui::SameLine();
    ImGui::Checkbox("Debug", &m_FilterDebug);

    ImGui::Separator();

    // ======================== ОБЛАСТЬ ЛОГОВ ========================
    float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("LogScrollRegion", ImVec2(0, -footerHeight), false,
                      ImGuiWindowFlags_HorizontalScrollbar);

    for (const auto& log : m_ConsoleLogs) {
        // Фильтрация по типу
        if (log.type == LogEntry::LOG_INFO    && !m_FilterInfo)    continue;
        if (log.type == LogEntry::LOG_WARNING && !m_FilterWarning) continue;
        if (log.type == LogEntry::LOG_ERROR   && !m_FilterError)   continue;
        if (log.type == LogEntry::LOG_DEBUG   && !m_FilterDebug)   continue;

        // Цвет по типу лога (как на референсе)
        ImVec4 color;
        const char* prefix;
        switch (log.type) {
            case LogEntry::LOG_INFO:
                color  = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);   // Светло-серый
                prefix = "[INFO]";
                break;
            case LogEntry::LOG_WARNING:
                color  = ImVec4(1.0f, 0.85f, 0.0f, 1.0f);  // Жёлтый
                prefix = "[WARNING]";
                break;
            case LogEntry::LOG_ERROR:
                color  = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);   // Красный
                prefix = "[ERROR]";
                break;
            case LogEntry::LOG_DEBUG:
                color  = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);   // Тёмно-серый
                prefix = "[DEBUG]";
                break;
            default:
                color  = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                prefix = "[LOG]";
                break;
        }

        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextWrapped("%s %s", prefix, log.message.c_str());
        ImGui::PopStyleColor();
    }

    // Автоскролл вниз при новых логах
    if (m_ScrollToBottom && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    // ======================== СТРОКА ВВОДА КОМАНД ========================
    ImGui::Separator();

    bool reclaimFocus = false;
    ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_EnterReturnsTrue;

    ImGui::SetNextItemWidth(-1); // На всю ширину
    if (ImGui::InputText("##ConsoleInput", m_CommandBuffer, sizeof(m_CommandBuffer), inputFlags)) {
        if (m_CommandBuffer[0] != '\0') {
            // TODO: Обработать введённую команду
            //       Например: парсинг команды, вызов нужной функции
            //       addLog(LogEntry::LOG_INFO, std::string("> ") + m_CommandBuffer);
            addLog(LogEntry::LOG_INFO, std::string("> ") + m_CommandBuffer);

            m_CommandBuffer[0] = '\0';
            reclaimFocus = true;
            m_ScrollToBottom = true;
        }
    }

    // Возвращаем фокус на поле ввода после Enter
    if (reclaimFocus) {
        ImGui::SetKeyboardFocusHere(-1);
    }

    ImGui::End();
}

// ==========================================================================
//  CONSOLE API
// ==========================================================================

void EditorUI::addLog(LogEntry::Type type, const std::string& message) {
    m_ConsoleLogs.push_back({ type, message });
    m_ScrollToBottom = true;
}

void EditorUI::clearLogs() {
    m_ConsoleLogs.clear();
}

void EditorUI::getViewportRect(int& outX, int& outY, int& outW, int& outH) {
    if (!m_Window) return;
    
    int screenW, screenH;
    glfwGetFramebufferSize(m_Window, &screenW, &screenH);

    float menuBarH = ImGui::GetFrameHeight();
    float totalH   = (float)screenH - menuBarH;
    float leftW    = (float)screenW * 0.18f;
    float rightW   = (float)screenW * 0.22f;
    float bottomH  = totalH * 0.38f;

    outX = (int)leftW;
    // В OpenGL ось Y идёт снизу вверх, поэтому нижняя граница кармана = высота нижних панелей
    outY = (int)bottomH;
    outW = (int)(screenW - leftW - rightW);
    // Высота кармана — это общая высота за вычетом нижних панелей (topH из кода панелей)
    outH = (int)(totalH * 0.62f);
}
