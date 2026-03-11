#include "util.h"
#include "app.h"

#include <cstdlib>
#include <algorithm>
#include <GLFW/glfw3.h>

namespace {
    App* g_app = nullptr;

    constexpr float kPlayerHeight = 1.8f;
    constexpr float kPlayerRadius = 0.3f;
    constexpr float kPlayerEyeHeight = 1.6f;
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

    const Keyboard kb = appPtr->inputManager.keyboard();

    glm::vec3 move(0.0f);
    glm::vec3 forward = appPtr->camera.front();
    glm::vec3 right = appPtr->camera.right();

    if (glm::length(forward) > 0.0f) forward = glm::normalize(forward);
    if (glm::length(right) > 0.0f) right = glm::normalize(right);

    glm::vec3 dir(0.0f);
    if (kb.isDown(GLFW_KEY_W)) dir += forward;
    if (kb.isDown(GLFW_KEY_S)) dir -= forward;
    if (kb.isDown(GLFW_KEY_D)) dir += right;
    if (kb.isDown(GLFW_KEY_A)) dir -= right;
    if (glm::length(dir) > 0.0f) {
        dir = glm::normalize(dir);
    }

    float speed = walkSpeed;
    move += dir * speed;

    if (kb.isDown(GLFW_KEY_SPACE)) {
        move.y += flyUpSpeed;
    }
    if (kb.isDown(GLFW_KEY_LEFT_SHIFT)) {
        move.y -= flyDownSpeed;
    }

    appPtr->camera.move(move);
   
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

float Util::playerHeight() {
    return kPlayerHeight;
}

float Util::playerEyeHeight() {
    return kPlayerEyeHeight;
}

float Util::playerRadius() {
    return kPlayerRadius;
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
