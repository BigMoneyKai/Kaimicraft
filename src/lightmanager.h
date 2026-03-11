#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "light/spotlight.h"
#include "light/dirlight.h"
#include "light/pointlight.h"

class Shader;

class LightManager {
public:
    void init();
    void upload(Shader& shader);

public:
    DirLight sunlight;
    std::vector<PointLight> pointlights;
    std::vector<SpotLight> spotlights;

};
