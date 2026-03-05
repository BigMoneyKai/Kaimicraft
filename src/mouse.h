#pragma once 

#include <iterator>

#ifdef __APPLE__
#include <algorithm>
#endif

constexpr float mouseSensitivity = 0.006f;

class Mouse {
public:
    static constexpr int MAX_BTNS = 16;
    void init();

    bool isDown(int btn) const;
    bool isPressed(int btn) const;
    bool isReleased(int btn) const;
        
    double x() const;
    double y() const;
    double deltaX() const;
    double deltaY() const;
    double scrollX() const;
    double scrollY() const;

    void setBtn(int btn, bool isDown); 
    void setPos(double x, double y); 
    void addScroll(double sx, double sy);
    void nextFrame();
    bool valid(int btn) const;
    
private:
    bool m_curr[MAX_BTNS]{};
    bool m_prev[MAX_BTNS]{};

    double m_x;
    double m_y;
    
    double m_lastX;
    double m_lastY;

    double m_dx;
    double m_dy;

    double m_scrollX;
    double m_scrollY;

    bool m_isFirstMove;
};

