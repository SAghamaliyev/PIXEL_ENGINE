#pragma once

#include "Core/InputSystem/InputSystem.h"
#include "Systems/Asset/AssetSystem.h"
#include "Systems/Render/RenderSystem.h"
#include "Systems/Scene/SceneSystem.h"
#include "Systems/UI/UISystem.h"

class Engine {
public:
    bool initialize();
    void run();
    void terminate();

private:
    // Subsystems and managers of program
    RenderSystem* m_pRenderSystem = nullptr;
    SceneSystem* m_pSceneSystem = nullptr;
    UISystem* m_pUiSystem = nullptr;
    AssetSystem* m_pAssetSystem = nullptr;
    InputManager* m_pInputManager = nullptr;

    // Main application window
    GLFWwindow* m_pWindow = nullptr;

    // Dimensions of the rendering viewport area
    int m_viewportWidth = 0;
    int m_viewportHeight = 0;

    // Processes the editor events that have occurred
    void processEditorEvents();

    // This function sends necessary information to UI module to visualize and show it
    void visualizeEditorEvents();
};
