#include "InputSystem.h"
#include <cstring>

void InputManager::UpdateKeyState(GLFWwindow* window) {

    std::memcpy(KeysPrevStates, KeysCurrentStates, sizeof(KeysCurrentStates));
    for (int i = 0; i <= (int)Keys::Tilde; i++) {
        KeysCurrentStates[i] = (int)glfwGetKey(window, glfwKeys[i]);
    }

    std::memcpy(MouseButtonsPrevStates, MouseButtonsCurrentStates, sizeof(MouseButtonsCurrentStates));
    for (int i = (int)MouseButtons::MouseLeft; i <= (int)MouseButtons::Mouse5; i++) {
        MouseButtonsCurrentStates[i] = (int)glfwGetMouseButton(window, glfwMouseButtons[i]);
    }

    double tempXpos = 0.0;
    double tempYpos = 0.0;

    double* xpos = &tempXpos;
    double* ypos = &tempYpos;

    glfwGetCursorPos(window, xpos, ypos);

    MousePrevPositions[0] = MouseCurrentPositions[0];
    MousePrevPositions[1] = MouseCurrentPositions[1];
    MouseCurrentPositions[0] = (float)tempXpos;
    MouseCurrentPositions[1] = (float)tempYpos;
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

bool InputManager::isMousePositionChanged(float& xpos, float& ypos) {
    if (MousePrevPositions[0] == MouseCurrentPositions[0]
        && MousePrevPositions[1] == MouseCurrentPositions[1]) {

        return false;
    }
    else {
        xpos = MouseCurrentPositions[0] - MousePrevPositions[0];
        ypos = MouseCurrentPositions[1] - MousePrevPositions[1];
        return true;
    }
}

void InputManager::analyzeInput(GLFWwindow* window) {
    UpdateKeyState(window);

    InputEditor();
}