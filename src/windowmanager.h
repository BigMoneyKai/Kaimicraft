#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

constexpr int default_width = 1280;
constexpr int default_height = 720;

typedef enum {
    FULLSCREEN,
    BORDERLESS,
    WINDOWED,
} DisplayMode;

class WindowManager {
public:
    void init(std::string title, DisplayMode display=BORDERLESS);
    void destroy();
    int getWidth() const;
    int getHeight() const;
    bool isHeadless() const;

public:
    GLFWwindow* window;
    int width = 0;
    int height = 0;
    bool headless = false;

private:
    const GLFWvidmode* mode;
    GLFWmonitor* monitor;

};
