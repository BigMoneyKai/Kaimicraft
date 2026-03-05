#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>

#include "debug.h"

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

public:
    GLFWwindow* window;
    int width = 0;
    int height = 0;

private:
    const GLFWvidmode* mode;
    GLFWmonitor* monitor;
    
};
