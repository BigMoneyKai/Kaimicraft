#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <algorithm>

#include "windowmanager.h"
#include "mouse.h"
#include "keyboard.h"

class InputManager {
public:
    void init(GLFWwindow* window);
    void poll();
    void nextFrame();

    const Mouse& mouse() const { return m_mouse; }
    const Keyboard& keyboard() const { return m_keyboard; };

    void onKey(int key, int action);
    void onMouseButton(int button, int action);
    void onCursorPos(double x, double y);
    void onScroll(double sx, double sy);

    void destroy();
private:
    GLFWwindow* m_window = nullptr;
    Mouse m_mouse;
    Keyboard m_keyboard;
};
