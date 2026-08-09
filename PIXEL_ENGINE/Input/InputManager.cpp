#include "InputManager.h"
#include <cstring>

int InputManager::glfwKeys[100] = {
    // Цифры
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

    // Буквы
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

    // Функциональные клавиши
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

    // Управляющие клавиши и модификаторы
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

    // Стрелки
    GLFW_KEY_RIGHT,
    GLFW_KEY_LEFT,
    GLFW_KEY_DOWN,
    GLFW_KEY_UP,

    // Дополнительные символы
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

int InputManager::glfwMouseButtons[100] = {GLFW_MOUSE_BUTTON_1,GLFW_MOUSE_BUTTON_2,
                                GLFW_MOUSE_BUTTON_3,GLFW_MOUSE_BUTTON_4,GLFW_MOUSE_BUTTON_5};

int InputManager::KeysCurrentStates[100] = {0};
int InputManager::KeysPrevStates[100] = {0};

int InputManager::MouseButtonsCurrentStates[100] = {0};
int InputManager::MouseButtonsPrevStates[100] = {0};

void InputManager::Update(GLFWwindow* window) {

    std::memcpy(KeysPrevStates, KeysCurrentStates, sizeof(KeysCurrentStates));
    for (int i = 0; i <= (int)Keys::Tilde; i++) {
        KeysCurrentStates[i] = (int)glfwGetKey(window, glfwKeys[i]);
    }

    std::memcpy(MouseButtonsPrevStates, MouseButtonsCurrentStates, sizeof(MouseButtonsCurrentStates));
    for (int i = (int)MouseButtons::MouseLeft; i <= (int)MouseButtons::Mouse5; i++) {
        MouseButtonsCurrentStates[i] = (int)glfwGetMouseButton(window, glfwMouseButtons[i]);
    }
}

bool InputManager::IsKeyJustPressed(Keys key) {
    if (KeysCurrentStates[key] == GLFW_PRESS && KeysPrevStates[key] == GLFW_RELEASE) {
        return true;
    }
    else {
        return false;
    }
}

bool InputManager::IsKeyJustReleased(Keys key) {
    if (KeysCurrentStates[key] == GLFW_RELEASE && KeysPrevStates[key] == GLFW_PRESS) {
        return true;
    }
    else {
        return false;
    }
}

bool InputManager::IsMouseButtonJustPressed(MouseButtons key) {
    if (MouseButtonsCurrentStates[key] == GLFW_PRESS && MouseButtonsPrevStates[key] == GLFW_RELEASE) {
        return true;
    }
    else {
        return false;
    }
}

bool InputManager::IsMouseButtonJustReleased(MouseButtons key) {
    if (MouseButtonsCurrentStates[key] == GLFW_RELEASE && MouseButtonsPrevStates[key] == GLFW_PRESS) {
        return true;
    }
    else {
        return false;
    }
}

bool InputManager::IsKeyHeld(Keys key) {
    if (KeysCurrentStates[key] == GLFW_PRESS && KeysPrevStates[key] == GLFW_PRESS) {
        return true;
    }
    else {
        return false;
    }
}

bool InputManager::IsMouseButtonHeld(MouseButtons key) {
    if (MouseButtonsCurrentStates[key] == GLFW_PRESS && MouseButtonsPrevStates[key] == GLFW_PRESS) {
        return true;
    }
    else {
        return false;
    }
}