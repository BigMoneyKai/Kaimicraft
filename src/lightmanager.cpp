#include "lightmanager.h"
#include "shader.h"

void LightManager::init() {
    // Default sunlight
    sunlight.direction = glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f));
    sunlight.ambient  = glm::vec3(0.2f, 0.2f, 0.2f);
    sunlight.diffuse  = glm::vec3(0.9f, 0.9f, 0.9f);
    sunlight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    pointlights.clear();
    spotlights.clear();

    // One default point light
    PointLight p{};
    p.position = glm::vec3(10.0f, 10.0f, 10.0f);
    p.ambient  = glm::vec3(0.05f, 0.05f, 0.05f);
    p.diffuse  = glm::vec3(0.8f, 0.8f, 0.8f);
    p.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    p.kc = 1.0f;
    p.kl = 0.09f;
    p.kq = 0.032f;
    pointlights.push_back(p);
}

void LightManager::upload(Shader& shader) {
    // Directional light
    shader.setVec("dirLight.direction", sunlight.direction);
    shader.setVec("dirLight.ambient",  sunlight.ambient);
    shader.setVec("dirLight.diffuse",  sunlight.diffuse);
    shader.setVec("dirLight.specular", sunlight.specular);

    // Point lights
    shader.setInt("numPointLights", (int)pointlights.size());
    for(size_t i = 0; i < pointlights.size(); i++) {
        const PointLight& p = pointlights[i];
        const std::string base = "pointLights[" + std::to_string(i) + "].";
        shader.setVec(base + "position", p.position);
        shader.setVec(base + "ambient",  p.ambient);
        shader.setVec(base + "diffuse",  p.diffuse);
        shader.setVec(base + "specular", p.specular);
        shader.setFloat(base + "kc", p.kc);
        shader.setFloat(base + "kl", p.kl);
        shader.setFloat(base + "kq", p.kq);
    }
}

