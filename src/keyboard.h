#pragma once

#include <iterator>

#ifdef __APPLE__
#include <algorithm>
#endif

class Keyboard {
public:
    static constexpr int MAX_KEYS = 512;
    void init();

    bool isDown(int key) const;
    bool isPressed(int key) const;
    bool isReleased(int key) const;
    
    void setKey(int key, bool isDown);
    void nextFrame();
    bool valid(int key) const;
private:
    bool m_curr[MAX_KEYS]{};
    bool m_prev[MAX_KEYS]{};
};


