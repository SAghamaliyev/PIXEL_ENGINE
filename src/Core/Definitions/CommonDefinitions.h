// Below are only small, dependency-free types that are used by several modules
// (actually I want to divide these structures in future to several files)

#pragma once

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
