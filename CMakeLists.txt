cmake_minimum_required(VERSION 3.20)

project(PIXEL_ENGINE
    VERSION 1.0.0
    LANGUAGES C CXX
)

# C++ Standard configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# C Standard configuration for glad
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile commands for language servers (clangd, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler-specific options
if(MSVC)
    add_compile_options(
        /MP             # Multi-processor compilation
        /utf-8          # UTF-8 source and execution charset
        /permissive-    # Strict conformance mode
        /W3             # Warning level 3
    )
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Paths to engine and third-party directories
set(ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/PIXEL_ENGINE")
set(SRC_DIR "${ENGINE_ROOT}/src")
set(THIRD_PARTY_DIR "${ENGINE_ROOT}/third_party")

# Engine source files
set(ENGINE_SOURCES
    # Core
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/Engine.h"
    "${SRC_DIR}/Engine.cpp"

    "${SRC_DIR}/Core/Definitions/CameraDefinitions.h"
    "${SRC_DIR}/Core/Definitions/CommonDefinitions.h"
    "${SRC_DIR}/Core/Definitions/EntityDefinitions.h"
    "${SRC_DIR}/Core/Definitions/MaterialDefinitions.h"
    "${SRC_DIR}/Core/Definitions/MeshDefinitions.h"
    "${SRC_DIR}/Core/Definitions/SceneDefinitions.h"

    "${SRC_DIR}/Core/EventSystem/EventSystem.h"
    "${SRC_DIR}/Core/EventSystem/EventSystem.cpp"
    "${SRC_DIR}/Core/EventSystem/EventTypes.h"

    "${SRC_DIR}/Core/InputSystem/InputSystem.h"
    "${SRC_DIR}/Core/InputSystem/InputSystem.cpp"
    "${SRC_DIR}/Core/InputSystem/EditorInput.cpp"

    "${SRC_DIR}/Core/Logger/Logger.h"
    "${SRC_DIR}/Core/Logger/Logger.cpp"

    # Systems / Asset
    "${SRC_DIR}/Systems/Asset/AssetSystem.h"
    "${SRC_DIR}/Systems/Asset/AssetSystem.cpp"
    "${SRC_DIR}/Systems/Asset/AssetSystemMesh.cpp"
    "${SRC_DIR}/Systems/Asset/AssetSystemTexture.cpp"

    # Systems / Render
    "${SRC_DIR}/Systems/Render/RenderSystem.h"
    "${SRC_DIR}/Systems/Render/RenderSystem.cpp"
    "${SRC_DIR}/Systems/Render/MeshManager/MeshManager.h"
    "${SRC_DIR}/Systems/Render/MeshManager/MeshManager.cpp"
    "${SRC_DIR}/Systems/Render/ShaderManager/ShaderManager.h"
    "${SRC_DIR}/Systems/Render/ShaderManager/ShaderManager.cpp"
    "${SRC_DIR}/Systems/Render/TextureManager/TextureManager.h"
    "${SRC_DIR}/Systems/Render/TextureManager/TextureManager.cpp"

    # Systems / Scene
    "${SRC_DIR}/Systems/Scene/SceneSystem.h"
    "${SRC_DIR}/Systems/Scene/SceneSystem.cpp"
    "${SRC_DIR}/Systems/Scene/SceneSystemCamera.cpp"
    "${SRC_DIR}/Systems/Scene/SceneSystemEntity.cpp"
    "${SRC_DIR}/Systems/Scene/CameraManager/CameraManager.h"
    "${SRC_DIR}/Systems/Scene/CameraManager/CameraManager.cpp"
    "${SRC_DIR}/Systems/Scene/EntityManager/EntityManager.h"
    "${SRC_DIR}/Systems/Scene/EntityManager/EntityManager.cpp"
    "${SRC_DIR}/Systems/Scene/TransformManager/TransformManager.h"
    "${SRC_DIR}/Systems/Scene/TransformManager/TransformManager.cpp"

    # Systems / UI
    "${SRC_DIR}/Systems/UI/UISystem.h"
    "${SRC_DIR}/Systems/UI/UISystem.cpp"
    "${SRC_DIR}/Systems/UI/EditorViews.h"
    "${SRC_DIR}/Systems/UI/Gizmo/Gizmo.h"
    "${SRC_DIR}/Systems/UI/Gizmo/Gizmo.cpp"
    "${SRC_DIR}/Systems/UI/Layout/EditorLayout.h"
    "${SRC_DIR}/Systems/UI/MenuBar/MenuBar.h"
    "${SRC_DIR}/Systems/UI/MenuBar/MenuBar.cpp"
    "${SRC_DIR}/Systems/UI/Panels/ConsolePanel.h"
    "${SRC_DIR}/Systems/UI/Panels/ConsolePanel.cpp"
    "${SRC_DIR}/Systems/UI/Panels/ContentBrowserPanel.h"
    "${SRC_DIR}/Systems/UI/Panels/ContentBrowserPanel.cpp"
    "${SRC_DIR}/Systems/UI/Panels/InspectorPanel.h"
    "${SRC_DIR}/Systems/UI/Panels/InspectorPanel.cpp"
    "${SRC_DIR}/Systems/UI/Panels/SceneHierarchyPanel.h"
    "${SRC_DIR}/Systems/UI/Panels/SceneHierarchyPanel.cpp"
    "${SRC_DIR}/Systems/UI/Panels/ViewportPanel.h"
    "${SRC_DIR}/Systems/UI/Panels/ViewportPanel.cpp"
    "${SRC_DIR}/Systems/UI/Theme/EditorTheme.h"
    "${SRC_DIR}/Systems/UI/Theme/EditorTheme.cpp"

    # ImGui (Materials)
    "${SRC_DIR}/Systems/UI/Materials/imconfig.h"
    "${SRC_DIR}/Systems/UI/Materials/imgui.h"
    "${SRC_DIR}/Systems/UI/Materials/imgui.cpp"
    "${SRC_DIR}/Systems/UI/Materials/imgui_draw.cpp"
    "${SRC_DIR}/Systems/UI/Materials/imgui_internal.h"
    "${SRC_DIR}/Systems/UI/Materials/imgui_tables.cpp"
    "${SRC_DIR}/Systems/UI/Materials/imgui_widgets.cpp"
    "${SRC_DIR}/Systems/UI/Materials/imstb_rectpack.h"
    "${SRC_DIR}/Systems/UI/Materials/imstb_textedit.h"
    "${SRC_DIR}/Systems/UI/Materials/imstb_truetype.h"
    "${SRC_DIR}/Systems/UI/Materials/Backends/imgui_impl_glfw.h"
    "${SRC_DIR}/Systems/UI/Materials/Backends/imgui_impl_glfw.cpp"
    "${SRC_DIR}/Systems/UI/Materials/Backends/imgui_impl_opengl3.h"
    "${SRC_DIR}/Systems/UI/Materials/Backends/imgui_impl_opengl3.cpp"
    "${SRC_DIR}/Systems/UI/Materials/Backends/imgui_impl_opengl3_loader.h"

    # Third-party sources
    "${THIRD_PARTY_DIR}/glad/src/glad.c"
    "${THIRD_PARTY_DIR}/ImGuizmo.h"
    "${THIRD_PARTY_DIR}/ImGuizmo.cpp"
    "${THIRD_PARTY_DIR}/stb_image.h"
)

# Define executable target
add_executable(PIXEL_ENGINE ${ENGINE_SOURCES})

# Target include directories
target_include_directories(PIXEL_ENGINE PRIVATE
    "${SRC_DIR}"
    "${THIRD_PARTY_DIR}"
    "${THIRD_PARTY_DIR}/include"
    "${THIRD_PARTY_DIR}/glfw/include"
    "${THIRD_PARTY_DIR}/glad/include"
    "${THIRD_PARTY_DIR}/glm-1.0.3"
    "${SRC_DIR}/Systems/UI/Materials"
    "${SRC_DIR}/Systems/UI/Materials/Backends"
)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(PIXEL_ENGINE PRIVATE OpenGL::GL)

# GLFW
find_package(glfw3 QUIET)
if(TARGET glfw)
    target_link_libraries(PIXEL_ENGINE PRIVATE glfw)
elseif(TARGET glfw3)
    target_link_libraries(PIXEL_ENGINE PRIVATE glfw3)
elseif(WIN32 AND MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
    target_link_libraries(PIXEL_ENGINE PRIVATE "${THIRD_PARTY_DIR}/glfw/lib/glfw3.lib")
else()
    find_package(glfw3 REQUIRED)
    target_link_libraries(PIXEL_ENGINE PRIVATE glfw)
endif()

# Windows platform libraries
if(WIN32)
    target_link_libraries(PIXEL_ENGINE PRIVATE comdlg32)
endif()

# Debugger working directory for Visual Studio
set_target_properties(PIXEL_ENGINE PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${ENGINE_ROOT}"
)

# Source group for IDE tree organization (Visual Studio, Xcode, CLion)
source_group(TREE "${ENGINE_ROOT}" PREFIX "Project" FILES ${ENGINE_SOURCES})

# Post-build: copy assets and configuration next to the executable
if(EXISTS "${ENGINE_ROOT}/assets")
    add_custom_command(TARGET PIXEL_ENGINE POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${ENGINE_ROOT}/assets"
            "$<TARGET_FILE_DIR:PIXEL_ENGINE>/assets"
        COMMENT "Copying assets to output directory..."
    )
endif()

if(EXISTS "${ENGINE_ROOT}/imgui.ini")
    add_custom_command(TARGET PIXEL_ENGINE POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ENGINE_ROOT}/imgui.ini"
            "$<TARGET_FILE_DIR:PIXEL_ENGINE>/imgui.ini"
        COMMENT "Copying imgui.ini to output directory..."
    )
endif()
