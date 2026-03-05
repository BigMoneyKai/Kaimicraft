#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cmath>
#include <random>

#include "debug.h"

class App;

namespace Util {
    glm::vec4 calcTileUV(int, int, int, int, int, int);
    void inputControl(App* appPtr);
    float randomNum(float min, float max);
}

