#pragma once

#include "Core/InputSystem/InputSystem.h"
#include "Core/Logger/Logger.h"
#include "Systems/Asset/AssetSystem.h"
#include "Systems/Render/RenderSystem.h"
#include "Systems/Scene/SceneSystem.h"
#include "Systems/UI/UISystem.h"

struct GLFWwindow;

class Engine {
private:
    RenderSystem* m_pRenderSystem;
    SceneSystem* m_pSceneSystem;
    UISystem* m_pUiSystem;
    AssetSystem* m_pAssetSystem;
    InputManager* m_pInputManager;
    GLFWwindow* m_pWindow;

    int m_viewportWidth = 0;
    int m_viewportHeight = 0;

    void processEditorEvents();
    void visualizeEditorEvents();
public:
    bool initialize();
    void run();
    void terminate();
};
