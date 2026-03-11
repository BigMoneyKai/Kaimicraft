#include "inputmanager.h"

static void keyCallback(GLFWwindow* w, int key, int, int action, int) {
    auto* input = static_cast<InputManager*>(glfwGetWindowUserPointer(w));
    if (input) input->onKey(key, action);
}

static void mouseButtonCallback(GLFWwindow* w, int btn, int action, int) {
    auto* input = static_cast<InputManager*>(glfwGetWindowUserPointer(w));
    if (input) input->onMouseButton(btn, action);
}

static void cursorPosCallback(GLFWwindow* w, double x, double y) {
    auto* input = static_cast<InputManager*>(glfwGetWindowUserPointer(w));
    if (input) input->onCursorPos(x, y);
}

static void scrollCallback(GLFWwindow* w, double sx, double sy) {
    auto* input = static_cast<InputManager*>(glfwGetWindowUserPointer(w));
    if (input) input->onScroll(sx, sy);
}

void InputManager::init(GLFWwindow* window) {
    m_window = window;
    m_mouse.init();
    m_keyboard.init();

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);

}

void InputManager::poll() {
    if (!m_window) return;

    for (int key = 0; key < Keyboard::MAX_KEYS; ++key) {
        int state = glfwGetKey(m_window, key);
        m_keyboard.setKey(key, state == GLFW_PRESS || state == GLFW_REPEAT);
    }

    for (int btn = 0; btn < Mouse::MAX_BTNS; ++btn) {
        int state = glfwGetMouseButton(m_window, btn);
        m_mouse.setBtn(btn, state == GLFW_PRESS);
    }
}

void InputManager::nextFrame() {
    m_mouse.nextFrame();
    m_keyboard.nextFrame();
}

void InputManager::onKey(int key, int action) {
    m_keyboard.setKey(key, action != GLFW_RELEASE);
}

void InputManager::onMouseButton(int btn, int action) {
    m_mouse.setBtn(btn, action != GLFW_RELEASE);
}

void InputManager::onCursorPos(double x, double y) {
    m_mouse.setPos(x, y);
}

void InputManager::onScroll(double sx, double sy) {
    m_mouse.addScroll(sx, sy);
}

void InputManager::destroy() {

}
