#include "windowmanager.h"

#include <cstdlib>
#include <cstring>

static bool isHeadlessEnv() {
    const char* headless = std::getenv("KAIMICRAFT_HEADLESS");
    if (headless && headless[0] != '\0' && std::strcmp(headless, "0") != 0) {
        return true;
    }
    const char* display = std::getenv("DISPLAY");
    const char* wayland = std::getenv("WAYLAND_DISPLAY");
    return (!display || display[0] == '\0') && (!wayland || wayland[0] == '\0');
}

void WindowManager::init(std::string title, DisplayMode display) {
    headless = isHeadlessEnv();
    if (headless) {
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_NULL);
    }
    if(!glfwInit()) {
        const char* desc = nullptr;
        int code = glfwGetError(&desc);
        if (!headless) {
            glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_NULL);
            if (glfwInit()) {
                headless = true;
                INFO("GLFW fallback to headless platform (code=%d): %s", code, desc ? desc : "no description");
            } else {
                const char* desc2 = nullptr;
                int code2 = glfwGetError(&desc2);
                FATAL("GLFW initialization failed (code=%d): %s", code2, desc2 ? desc2 : "no description");
            }
        } else {
            FATAL("GLFW initialization failed (code=%d): %s", code, desc ? desc : "no description");
        }
    }
    INFO("GLFW version: %s", glfwGetVersionString());

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    monitor = nullptr;
    if (!headless) {
        monitor = glfwGetPrimaryMonitor();
        if(!monitor) {
            FATAL("Primary monitor getting failed");
        }
    }


    const DisplayMode resolvedDisplay = headless ? WINDOWED : display;
    switch(resolvedDisplay) {
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

bool WindowManager::isHeadless() const {
    return headless;
}
