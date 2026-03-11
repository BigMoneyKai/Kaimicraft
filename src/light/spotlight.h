#pragma once

#include <glm/glm.hpp>

struct SpotLight {
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float kc; // constant
    float kl; // linear
    float kq; // quadratic
};
