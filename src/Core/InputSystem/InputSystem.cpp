#include "InputSystem.h"

#include <cstring>

#include <GLFW/glfw3.h>

void InputManager::updateKeyState(GLFWwindow* window) {

    std::memcpy(m_keysPreviousStates, m_keysCurrentStates, sizeof(m_keysCurrentStates));
    for (int i = 0; i <= (int)Key::Tilde; i++) {
        m_keysCurrentStates[i] = (int)glfwGetKey(window, m_glfwKeys[i]);
    }

    std::memcpy(m_mouseButtonsPreviousStates, m_mouseButtonsCurrentStates, sizeof(m_mouseButtonsCurrentStates));
    for (int i = (int)MouseButton::MouseLeft; i <= (int)MouseButton::Mouse5; i++) {
        m_mouseButtonsCurrentStates[i] = (int)glfwGetMouseButton(window, m_glfwMouseButtons[i]);
    }

    double temp_x_pos = 0.0;
    double temp_y_pos = 0.0;
    double* x_pos = &temp_x_pos;
    double* y_pos = &temp_y_pos;

    glfwGetCursorPos(window, x_pos, y_pos);

    m_mousePreviousPositions[0] = m_mouseCurrentPositions[0];
    m_mousePreviousPositions[1] = m_mouseCurrentPositions[1];
    m_mouseCurrentPositions[0] = (float)temp_x_pos;
    m_mouseCurrentPositions[1] = (float)temp_y_pos;
}

bool InputManager::isKeyJustPressed(Key key) {
    int index = static_cast<int>(key);

    if (m_keysCurrentStates[index] == GLFW_PRESS && m_keysPreviousStates[index] == GLFW_RELEASE) {
        return true;
    }
    else {
        return false;
    }
}

bool InputManager::isKeyJustReleased(Key key) {
    int index = static_cast<int>(key);

    if (m_keysCurrentStates[index] == GLFW_RELEASE && m_keysPreviousStates[index] == GLFW_PRESS) {
        return true;
    }
    else {
        return false;
    }
}

bool InputManager::isMouseButtonJustPressed(MouseButton key) {
    int index = static_cast<int>(key);

    if (m_mouseButtonsCurrentStates[index] == GLFW_PRESS 
        && m_mouseButtonsPreviousStates[index] == GLFW_RELEASE) {

        return true;
    }
    else {
        return false;
    }
}

bool InputManager::isMouseButtonJustReleased(MouseButton key) {
    int index = static_cast<int>(key);

    if (m_mouseButtonsCurrentStates[index] == GLFW_RELEASE 
        && m_mouseButtonsPreviousStates[index] == GLFW_PRESS) {
       
        return true;
    }
    else {
        return false;
    }
}

bool InputManager::isKeyHeld(Key key) {
    int index = static_cast<int>(key);

    if (m_keysCurrentStates[index] == GLFW_PRESS 
        && m_keysPreviousStates[index] == GLFW_PRESS) {
        return true;
    }
    else {
        return false;
    }
}

bool InputManager::isMouseButtonHeld(MouseButton key) {
    int index = static_cast<int>(key);

    if (m_mouseButtonsCurrentStates[index] == GLFW_PRESS && m_mouseButtonsPreviousStates[index] == GLFW_PRESS) {
        return true;
    }
    else {
        return false;
    }
}

bool InputManager::isMousePositionChanged() {
    if (m_mousePreviousPositions[0] == m_mouseCurrentPositions[0]
        && m_mousePreviousPositions[1] == m_mouseCurrentPositions[1]) {

        return false;
    }
    else {
        return true;
    }
}

std::vector<float> InputManager::getMouseOffsets() {
    float x_offset = m_mouseCurrentPositions[0] - m_mousePreviousPositions[0];
    float y_offset = m_mouseCurrentPositions[1] - m_mousePreviousPositions[1];
    return {x_offset, y_offset};
}

void InputManager::processInput(GLFWwindow* window) {
    updateKeyState(window);
    processEditorInput();
}