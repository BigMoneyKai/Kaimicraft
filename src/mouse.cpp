#include "mouse.h"

// Mouse
void Mouse::init() {
    m_x = 0.0;
    m_y = 0.0;

    m_lastX = 0.0;
    m_lastY = 0.0;

    m_dx = 0.0;
    m_dy = 0.0;

    m_scrollX = 0.0;
    m_scrollY = 0.0;
    
    m_isFirstMove = true;
}

bool Mouse::isDown(int btn) const {
    return valid(btn) && m_curr[btn];
}

bool Mouse::isPressed(int btn) const {
    return valid(btn) && m_curr[btn] && !m_prev[btn];
}

bool Mouse::isReleased(int btn) const {
    return valid(btn) && !m_curr[btn] && m_prev[btn];
}

double Mouse::x() const {
    return m_x;
}

double Mouse::y() const {
    return m_y;
}

double Mouse::deltaX() const {
    return m_dx;
}

double Mouse::deltaY() const {
    return m_dy;
}

double Mouse::scrollX() const {
    return m_scrollX;
}

double Mouse::scrollY() const {
    return m_scrollY;
}

void Mouse::setBtn(int btn, bool isDown) {
    if(valid(btn)) {
        m_curr[btn] = isDown;
    }
}

void Mouse::setPos(double x, double y) {
    m_x = x;
    m_y = y;
    if(m_isFirstMove) {
        m_lastX = x;
        m_lastY = y;

        m_isFirstMove = false;
        m_dx = 0.0;
        m_dy = 0.0;

        return; 
    }

    m_dx = x - m_lastX;
    m_dy = m_lastY - y;

    m_lastX = x;
    m_lastY = y;

}

void Mouse::addScroll(double sx, double sy) {
    m_scrollX += sx;
    m_scrollY += sy;
}

void Mouse::nextFrame() {
    std::copy(std::begin(m_curr), std::end(m_curr), std::begin(m_prev));

    // clear every frame
    m_dx = 0.0;
    m_dy = 0.0;
    m_scrollX = 0.0;
    m_scrollY = 0.0;
}

bool Mouse::valid(int btn) const {
    return 0 <= btn && btn < MAX_BTNS;
}


