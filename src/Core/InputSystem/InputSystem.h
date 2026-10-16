#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/EventSystem/EventSystem.h"

class InputManager {

    // Engine's own keyboard key names (values match GLFW codes)
    enum class Key {

        Num0,
        Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

        Escape,
        Tab,
        CapsLock,
        LeftShift,
        RightShift,
        LeftControl,
        RightControl,
        LeftAlt,
        RightAlt,
        Space,
        Enter,
        Backspace,
        Insert,
        Delete,
        Home,
        End,
        PageUp,
        PageDown,

        Right,
        Left, Down, Up,


        Minus,
        Equal,
        LeftBracket,
        RightBracket,
        Semicolon,
        Apostrophe,
        Comma,
        Period,
        Slash,
        Backslash,
        Tilde
    };

    // Engine's own mouse button names (values match GLFW codes)
    enum class MouseButton {
        MouseLeft = GLFW_MOUSE_BUTTON_LEFT,
        MouseRight = GLFW_MOUSE_BUTTON_RIGHT,
        MouseMiddle = GLFW_MOUSE_BUTTON_MIDDLE,
        Mouse4 = GLFW_MOUSE_BUTTON_4,
        Mouse5 = GLFW_MOUSE_BUTTON_5
    };

    // GLFW doesn't understand engine's own homemade keys/mouse buttons,
    // so we map them to GLFW key codes (the order must match the Key enum)
    int m_glfwKeys[100] = {
        // Numbers
        GLFW_KEY_0,
        GLFW_KEY_1,
        GLFW_KEY_2,
        GLFW_KEY_3,
        GLFW_KEY_4,
        GLFW_KEY_5,
        GLFW_KEY_6,
        GLFW_KEY_7,
        GLFW_KEY_8,
        GLFW_KEY_9,

        // Letters
        GLFW_KEY_A,
        GLFW_KEY_B,
        GLFW_KEY_C,
        GLFW_KEY_D,
        GLFW_KEY_E,
        GLFW_KEY_F,
        GLFW_KEY_G,
        GLFW_KEY_H,
        GLFW_KEY_I,
        GLFW_KEY_J,
        GLFW_KEY_K,
        GLFW_KEY_L,
        GLFW_KEY_M,
        GLFW_KEY_N,
        GLFW_KEY_O,
        GLFW_KEY_P,
        GLFW_KEY_Q,
        GLFW_KEY_R,
        GLFW_KEY_S,
        GLFW_KEY_T,
        GLFW_KEY_U,
        GLFW_KEY_V,
        GLFW_KEY_W,
        GLFW_KEY_X,
        GLFW_KEY_Y,
        GLFW_KEY_Z,

        // Functional Keys
        GLFW_KEY_F1,
        GLFW_KEY_F2,
        GLFW_KEY_F3,
        GLFW_KEY_F4,
        GLFW_KEY_F5,
        GLFW_KEY_F6,
        GLFW_KEY_F7,
        GLFW_KEY_F8,
        GLFW_KEY_F9,
        GLFW_KEY_F10,
        GLFW_KEY_F11,
        GLFW_KEY_F12,

        // Control Keys
        GLFW_KEY_ESCAPE,
        GLFW_KEY_TAB,
        GLFW_KEY_CAPS_LOCK,
        GLFW_KEY_LEFT_SHIFT,
        GLFW_KEY_RIGHT_SHIFT,
        GLFW_KEY_LEFT_CONTROL,
        GLFW_KEY_RIGHT_CONTROL,
        GLFW_KEY_LEFT_ALT,
        GLFW_KEY_RIGHT_ALT,
        GLFW_KEY_SPACE,
        GLFW_KEY_ENTER,
        GLFW_KEY_BACKSPACE,
        GLFW_KEY_INSERT,
        GLFW_KEY_DELETE,
        GLFW_KEY_HOME,
        GLFW_KEY_END,
        GLFW_KEY_PAGE_UP,
        GLFW_KEY_PAGE_DOWN,

        // Arrows
        GLFW_KEY_RIGHT,
        GLFW_KEY_LEFT,
        GLFW_KEY_DOWN,
        GLFW_KEY_UP,

        // Additional keys
        GLFW_KEY_MINUS,
        GLFW_KEY_EQUAL,
        GLFW_KEY_LEFT_BRACKET,
        GLFW_KEY_RIGHT_BRACKET,
        GLFW_KEY_SEMICOLON,
        GLFW_KEY_APOSTROPHE,
        GLFW_KEY_COMMA,
        GLFW_KEY_PERIOD,
        GLFW_KEY_SLASH,
        GLFW_KEY_BACKSLASH,
        GLFW_KEY_GRAVE_ACCENT
    };
    int m_glfwMouseButtons[100] = {
        GLFW_MOUSE_BUTTON_1,
        GLFW_MOUSE_BUTTON_2,
        GLFW_MOUSE_BUTTON_3,
        GLFW_MOUSE_BUTTON_4,
        GLFW_MOUSE_BUTTON_5
    };

    int m_keysCurrentStates[100] = {0};
    int m_keysPreviousStates[100] = {0};

    int m_mouseButtonsCurrentStates[100] = {0};
    int m_mouseButtonsPreviousStates[100] = {0};

    float m_mouseCurrentPositions[2] = {0};
    float m_mousePreviousPositions[2] = {0};

    // Below are homemade functions for getting state of keys/mouse buttons

    // Is key/mouse_button being held
    bool isKeyHeld(Key key);
    bool isMouseButtonHeld(MouseButton button);

    // Was key/mouse_button pressed in current frame
    bool isKeyJustPressed(Key key);
    bool isMouseButtonJustPressed(MouseButton button);

    // Was key/mouse_button released in current frame
    bool isKeyJustReleased(Key key);
    bool isMouseButtonJustReleased(MouseButton button);

    // Was the mouse moved by the user
    bool isMousePositionChanged();
    std::vector<float> getMouseOffsets(); // If mouse was moved, this function returns offsets (x, y)

    // Updates the current and previous state of keys and mouse buttons
    void updateKeyState(GLFWwindow* window);

    // Process all inputs in Engine (editor mode)
    void processEditorInput();

public:
    // This function just updates window's resolution and calls processEditorInput()
    void processInput(GLFWwindow* window);
};



