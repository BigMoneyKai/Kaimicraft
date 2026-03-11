#pragma once

#include <glm/glm.hpp>

#include "light/dirlight.h"

constexpr int noon = 0.1 * 60 * 60;
constexpr int evening = 0.1 * 60 * 60;
constexpr int night = 0.1 * 60 * 60;
constexpr int morning = 0.1 * 60 * 60;

enum class DayPhase {
    Morning,
    Noon,
    Evening,
    Night
};

class TimeManager {
public:
    void init();
    void destroy();
    void update();

    DayPhase phase() const;
    float phaseT() const;
    const glm::vec3& skyColor() const;
    const DirLight& sunlight() const;
private:
    int gameDuration;
    double startTime = 0.0;
    double dayTime = 0.0;
    DayPhase currentPhase = DayPhase::Morning;
    float currentPhaseT = 0.0f;
    glm::vec3 currentSkyColor = glm::vec3(0.529f, 0.808f, 0.922f);
    DirLight currentSunlight{};
};
