// Please put here some basic, ordinary structures and enum classes

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
