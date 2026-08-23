/* Данный Файл это своего рода реестр всего дерьма что нужно упростить, здесь находятся все самодельные
   мои функции работающие поверх функций glfw а также самодельные обозначения кнопок*/
#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

class InputManager {
public:
    enum Keys {
        // Цифры (на верхнем ряду: GLFW_KEY_0 и далее по порядку)
        Num0,
        Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

        // Буквы латинского алфавита (QWERTY: GLFW_KEY_A и далее по порядку)
        A,B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        // Функциональные клавиши
        F1,F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

        // Управляющие клавиши и модификаторы
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

        // Стрелки
        Right,
        Left, Down, Up,
        // Дополнительные символы
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

    enum MouseButtons {
        MouseLeft = GLFW_MOUSE_BUTTON_LEFT,   // равно 0
        MouseRight = GLFW_MOUSE_BUTTON_RIGHT,  // равно 1
        MouseMiddle = GLFW_MOUSE_BUTTON_MIDDLE, // равно 2
        Mouse4 = GLFW_MOUSE_BUTTON_4,      // равно 3
        Mouse5 = GLFW_MOUSE_BUTTON_5       // равно 4
    };

    

    // Это мои собственные функции для получения инфы о состоянии клавиши

    // 1. Удерживается ли кнопка прямо сейчас? (для пулемета / бега)
    static bool IsKeyHeld(Keys key);
    static bool IsMouseButtonHeld(MouseButtons button);

    // 2. Была ли кнопка нажата ИМЕННО В ЭТОМ кадре? (для одиночного выстрела / прыжка)
    static bool IsKeyJustPressed(Keys key);
    static bool IsMouseButtonJustPressed(MouseButtons button);

    // 3. Была ли кнопка отпущена ИМЕННО В ЭТОМ кадре?
    static bool IsKeyJustReleased(Keys key);
    static bool IsMouseButtonJustReleased(MouseButtons button);
    
    static void Update(GLFWwindow* window);


private:
    static int glfwKeys[100];
    static int glfwMouseButtons[100];

    static int KeysCurrentStates[100];
    static int KeysPrevStates[100];

    static int MouseButtonsCurrentStates[100];
    static int MouseButtonsPrevStates[100];
};



