#pragma once

#include "Core/Definitions/SceneDefinitions.h"
#include "Systems/Render/MeshManager/MeshManager.h"
#include "Systems/Render/ShaderManager/ShaderManager.h"
#include "Systems/Render/TextureManager/TextureManager.h"

class RenderSystem {
private:
    ShaderManager m_shaderManager;
    MeshManager m_meshManager;
    TextureManager m_textureManager;
public:
    void initializeRender();
    void renderScene(const SceneInfo& sceneInfo);
};
