#include "RenderSystem.h"

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Core/Definitions/EntityDefinitions.h"
#include "Core/Definitions/SceneDefinitions.h"
#include "Core/Logger/Logger.h"
#include "glm/gtc/type_ptr.hpp"
#include "Systems/Render/MeshManager/MeshManager.h"
#include "Systems/Render/ShaderManager/ShaderManager.h"
#include "Systems/Render/TextureManager/TextureManager.h"

void RenderSystem::initializeRender() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderSystem::renderScene(const SceneInfo& sceneInfo) {

    const CameraUnit& active_camera = sceneInfo.cameraList.at(sceneInfo.activeCameraID);

    for (const auto& it : sceneInfo.entityList) {
        const EntityUnit& entity = it.second;

        MeshInfo entity_mesh = m_meshManager.getMesh(entity.meshID,
                                                     entity.isActive);

        // We must pass entity even if it's deactivated, because textureManager must remove it from its Map
        unsigned int entity_texture = m_textureManager.getTexture(entity.textureID,
                                                                  entity.isActive);

        // If texture is equal to 0, it means entity is deactivated and texture for it is not valid anymore
        if (entity_texture == 0) {
            continue;
        }

        initializeRender();

        unsigned int entity_shader = m_shaderManager.getShader(entity.materialType);
        bool is_color_tint_enabled = entity.isColorTintEnabled;
        Color entity_color = entity.color;


        // Locations of uniform attributes in entity's shader
        int color_tint_location = glGetUniformLocation(entity_shader, "isColorTintEnabled");
        int color_location = glGetUniformLocation(entity_shader, "color");
        int texture_location = glGetUniformLocation(entity_shader, "texture2D");
        int transform_matrix_location = glGetUniformLocation(entity_shader, "transformMatrix");
        int view_matrix_location = glGetUniformLocation(entity_shader, "viewMatrix");
        int projection_matrix_location = glGetUniformLocation(entity_shader, "projectionMatrix");


        glUseProgram(entity_shader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, entity_texture);
        glBindVertexArray(entity_mesh.vao);

        // Send color related variables to entity's shader
        glUniform1i(color_tint_location, is_color_tint_enabled);
        glUniform4f(color_location, entity_color.r, entity_color.g, entity_color.b, entity_color.a);

        // Send texture to entity's shader
        glUniform1i(texture_location, 0);

        // Send all matrices to entity's shader
        glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, value_ptr(entity.transform.matrix));
        glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, value_ptr(active_camera.view));
        glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, value_ptr(active_camera.projection));

        glDrawElements(GL_TRIANGLES, entity_mesh.indexCount, GL_UNSIGNED_INT, 0);
    }
}