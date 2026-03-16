#include "timemanager.h"

#include <GLFW/glfw3.h>

#include <cmath>

namespace {
struct TimeKey {
    glm::vec3 sky;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t) {
    return a + (b - a) * t;
}
} // namespace

void TimeManager::init() {
    gameDuration = morning + noon + evening + night;
    startTime = glfwGetTime();
    dayTime = 0.0;
    currentPhase = DayPhase::Morning;
    currentPhaseT = 0.0f;
    currentSunlight.direction = glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f));
    currentSunlight.ambient  = glm::vec3(0.2f, 0.2f, 0.2f);
    currentSunlight.diffuse  = glm::vec3(0.9f, 0.9f, 0.9f);
    currentSunlight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

void TimeManager::destroy() {

}

void TimeManager::update() {
    if (gameDuration <= 0) {
        return;
    }

    double now = glfwGetTime();
    double elapsed = now - startTime;
    dayTime = std::fmod(elapsed, (double)gameDuration);
    if (dayTime < 0.0) {
        dayTime += gameDuration;
    }

    int tMorningEnd = morning;
    int tNoonEnd = morning + noon;
    int tEveningEnd = morning + noon + evening;

    if (dayTime < tMorningEnd) {
        currentPhase = DayPhase::Morning;
        currentPhaseT = morning > 0 ? (float)(dayTime / morning) : 0.0f;
    } else if (dayTime < tNoonEnd) {
        currentPhase = DayPhase::Noon;
        currentPhaseT = noon > 0 ? (float)((dayTime - tMorningEnd) / noon) : 0.0f;
    } else if (dayTime < tEveningEnd) {
        currentPhase = DayPhase::Evening;
        currentPhaseT = evening > 0 ? (float)((dayTime - tNoonEnd) / evening) : 0.0f;
    } else {
        currentPhase = DayPhase::Night;
        currentPhaseT = night > 0 ? (float)((dayTime - tEveningEnd) / night) : 0.0f;
    }

    TimeKey keys[4];
    keys[(int)DayPhase::Morning] = {
        glm::vec3(0.82f, 0.62f, 0.45f),
        glm::vec3(0.20f, 0.18f, 0.16f),
        glm::vec3(0.75f, 0.65f, 0.55f),
        glm::vec3(0.90f, 0.80f, 0.70f)
    };
    keys[(int)DayPhase::Noon] = {
        glm::vec3(0.529f, 0.808f, 0.922f),
        glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.95f, 0.95f, 0.95f),
        glm::vec3(1.00f, 1.00f, 1.00f)
    };
    keys[(int)DayPhase::Evening] = {
        glm::vec3(0.92f, 0.46f, 0.22f),
        glm::vec3(0.16f, 0.12f, 0.10f),
        glm::vec3(0.60f, 0.45f, 0.35f),
        glm::vec3(0.75f, 0.55f, 0.45f)
    };
    keys[(int)DayPhase::Night] = {
        glm::vec3(0.02f, 0.02f, 0.06f),
        glm::vec3(0.04f, 0.04f, 0.06f),
        glm::vec3(0.08f, 0.08f, 0.12f),
        glm::vec3(0.10f, 0.10f, 0.16f)
    };

    int cur = (int)currentPhase;
    int next = (cur + 1) % 4;
    const TimeKey& a = keys[cur];
    const TimeKey& b = keys[next];
    float t = currentPhaseT;

    currentSkyColor = lerp(a.sky, b.sky, t);
    currentSunlight.ambient  = lerp(a.ambient, b.ambient, t);
    currentSunlight.diffuse  = lerp(a.diffuse, b.diffuse, t);
    currentSunlight.specular = lerp(a.specular, b.specular, t);

    float dayT = (float)(dayTime / gameDuration);
    float angle = dayT * 6.2831853f;

    // Keep sunlight above the horizon so upward-facing surfaces receive directional diffuse.
    // The shader uses: lightDir = normalize(-dirLight.direction)
    float y = std::sin(angle);
    if (y < 0.05f) y = 0.05f;

    currentSunlight.direction = glm::normalize(glm::vec3(std::cos(angle), -y, -0.3f));
}

DayPhase TimeManager::phase() const {
    return currentPhase;
}

float TimeManager::phaseT() const {
    return currentPhaseT;
}

const glm::vec3& TimeManager::skyColor() const {
    return currentSkyColor;
}

const DirLight& TimeManager::sunlight() const {
    return currentSunlight;
}
