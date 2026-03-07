#include "util.h"
#include "app.h"

#include <cstdlib>

namespace {
    App* g_app = nullptr;
}

glm::vec4 Util::calcTileUV(int tileX, int tileY, int stride, int padding, int atlasWidth, int atlasHeight) {
    float u0 = tileX * (stride + padding) / (float)atlasWidth;
    float v0 = (stride + tileY * (stride + padding)) / (float)atlasHeight;
    float u1 = (stride + tileX * (stride + padding)) / (float)atlasWidth;
    float v1 = tileY * (stride + padding) / (float)atlasHeight;
    return {u0, v0, u1, v1};
}

void Util::inputControl(App* appPtr) {
    if(appPtr->inputManager.keyboard().isDown(GLFW_KEY_ESCAPE)) {
        appPtr->isRunning = false;
    }

    if(appPtr->inputManager.keyboard().isDown(GLFW_KEY_W)) {
        appPtr->camera.addPosX(-walkSpeed * cos(glm::radians(180.0f) + appPtr->camera.yaw()));
        appPtr->camera.addPosZ(-walkSpeed * sin(glm::radians(180.0f) + appPtr->camera.yaw()));
    }
    if(appPtr->inputManager.keyboard().isDown(GLFW_KEY_S)) {
        appPtr->camera.addPosX(-walkSpeed * cos(appPtr->camera.yaw()));
        appPtr->camera.addPosZ(-walkSpeed * sin(appPtr->camera.yaw()));
    }
    if(appPtr->inputManager.keyboard().isDown(GLFW_KEY_A)) {
        appPtr->camera.addPosX(-walkSpeed * cos(glm::radians(90.0f) + appPtr->camera.yaw()));
        appPtr->camera.addPosZ(-walkSpeed * sin(glm::radians(90.0f) + appPtr->camera.yaw()));
    }
    if(appPtr->inputManager.keyboard().isDown(GLFW_KEY_D)) {
        appPtr->camera.addPosX(-walkSpeed * cos(-glm::radians(90.0f) + appPtr->camera.yaw()));
        appPtr->camera.addPosZ(-walkSpeed * sin(-glm::radians(90.0f) + appPtr->camera.yaw()));
    }
    if(appPtr->inputManager.keyboard().isDown(GLFW_KEY_LEFT_SHIFT)) {
        appPtr->camera.addPosY(-flyDownSpeed); 
    }
    if(appPtr->inputManager.keyboard().isDown(GLFW_KEY_SPACE)) {
        appPtr->camera.addPosY(flyUpSpeed); 
    }
   
    // ===== ⭐ mouse vision =====
    double dx = appPtr->inputManager.mouse().deltaX();
    double dy = appPtr->inputManager.mouse().deltaY();

    appPtr->camera.addYaw(dx * mouseSensitivity);
    appPtr->camera.addPitch(dy * mouseSensitivity);

    // 防止万向锁
    appPtr->camera.setPitch(std::clamp(appPtr->camera.pitch(), glm::radians(-89.0f), glm::radians(89.0f)));

    appPtr->camera.updateVectors();

    // ===== 鼠标点击（可选）=====
    if (appPtr->inputManager.mouse().isPressed(GLFW_MOUSE_BUTTON_1)) {
        // 左键点击瞬间触发
    }

    if (appPtr->inputManager.mouse().isDown(GLFW_MOUSE_BUTTON_2)) {
        // 右键按住持续触发
    }

    // ===== 滚轮（可选：缩放/FOV）=====
    double scrollY = appPtr->inputManager.mouse().scrollY();
    if (scrollY != 0.0) {
        appPtr->camera.addFov(-scrollY);
        appPtr->camera.setFov(std::clamp(appPtr->camera.fov(), glm::radians(30.0f), glm::radians(150.0f)));
    }
}

float Util::randomNum(float min, float max) {
    if(min > max) {
        ERROR("Minimal number greater than maximal number");
        WARNING("Using minimal number as the return random number");
        return min;
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    std::uniform_real_distribution<float> dist(min, max); 
    return dist(gen);
}

void Util::quit(int code) {
    if (g_app) {
        g_app->shutdown();
    } else {
        glfwTerminate();
    }
    std::exit(code);
}

void Util::registerApp(App* appPtr) {
    g_app = appPtr;
}
