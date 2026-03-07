#include "windowmanager.h"

void WindowManager::init(std::string title, DisplayMode display) {
    if(!glfwInit()) {
        glfwTerminate();
        FATAL("GLFW initialization failed"); 
    }
    INFO("GLFW version: %s", glfwGetVersionString());

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    monitor = glfwGetPrimaryMonitor();
    if(!monitor) {
        glfwTerminate();
        FATAL("Primary monitor getting failed");
    }


    switch(display) {
        case FULLSCREEN:
            mode = glfwGetVideoMode(monitor);
            width = mode->width;
            height = mode->height;
            window = glfwCreateWindow(width, height, title.c_str(), monitor, NULL);
            break;

        case BORDERLESS:
            mode = glfwGetVideoMode(monitor);
            width = mode->width;
            height = mode->height;
            window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
            break;

        case WINDOWED:
            mode = NULL;
            width = default_width;
            height = default_height;
            window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
            break;
        default: 
            break;
    }
    if(!window) {
        FATAL("Window creating failed");
    }
    glfwMakeContextCurrent(window); 
}

void WindowManager::destroy() {
    if(window) glfwDestroyWindow(window);
    glfwTerminate();
}

int WindowManager::getWidth() const {
    return width;
}

int WindowManager::getHeight() const {
    return height;
}
