// Below are only small, dependency-free types that are used by several modules
// (actually I want to divide these structures in future to several files)

#pragma once

#include <cstdint>

struct Color {
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
};

enum class Movement {
    Forward,
    Back,
    Right,
    Left,
    Up,
    Down
};

enum class GizmoOperation {
    Translate,
    Rotate,
    Scale
};

constexpr uint64_t TEXTURE_DEFAULT_ID = 0;  // Default texture used for rendering
constexpr uint64_t MESH_DEFAULT_ID = 0;     // Default cube mesh used for rendering
constexpr long NO_ENTITY_ID = 0;            // No entity is selected

constexpr long CAMERA_DEFAULT = 1;          // This camera is main one, it cannot be removed and changed
