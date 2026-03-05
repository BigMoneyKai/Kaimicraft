#include "keyboard.h"

// Keyboard
void Keyboard::init() {

}

bool Keyboard::isDown(int key) const {
    return valid(key) && m_curr[key];
}

bool Keyboard::isPressed(int key) const {
    return valid(key) && m_curr[key] && !m_prev[key];
}

bool Keyboard::isReleased(int key) const {
    return valid(key) && !m_curr[key] && m_prev[key];
}

void Keyboard::setKey(int key, bool isDown) {
    if(valid(key)) m_curr[key] = isDown;
}

void Keyboard::nextFrame() {
    std::copy(std::begin(m_curr), std::end(m_curr), std::begin(m_prev));
}

bool Keyboard::valid(int key) const {
    return 0 <= key && key < MAX_KEYS;
}

