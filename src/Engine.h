#pragma once

#include "Core/InputSystem/InputSystem.h"
#include "Systems/Asset/AssetSystem.h"
#include "Systems/Render/RenderSystem.h"
#include "Systems/Scene/SceneSystem.h"
#include "Systems/UI/UISystem.h"

class Engine {
    RenderSystem* m_pRenderSystem = nullptr;
    SceneSystem* m_pSceneSystem = nullptr;
    UISystem* m_pUiSystem = nullptr;
    AssetSystem* m_pAssetSystem = nullptr;
    InputManager* m_pInputManager = nullptr;
    GLFWwindow* m_pWindow = nullptr;

    int m_viewportWidth = 0;
    int m_viewportHeight = 0;

    void processEditorEvents();
    void visualizeEditorEvents();
public:
    bool initialize();
    void run();
    void terminate();
};
