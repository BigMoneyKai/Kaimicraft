#include "perlinnoise.h"

float PerlinNoise::noise2d(float x, float z) {
    float x0 = std::floor(x);
    float z0 = std::floor(z);

    int x1 = x0 + 1;
    int z1 = z0 + 1;

    float sx = x - x0;
    float sz = z - z0;

    glm::vec2 g00 = grad2d(x0, z0);
    glm::vec2 g10 = grad2d(x1, z0);
    glm::vec2 g01 = grad2d(x0, z1);
    glm::vec2 g11 = grad2d(x1, z1);

    float n00 = glm::dot(g00, glm::vec2(sx, sz));
    float n10 = glm::dot(g10, glm::vec2(sx-1.0f, sz));
    float n01 = glm::dot(g01, glm::vec2(sx, sz-1.0f));
    float n11 = glm::dot(g11, glm::vec2(sx-1.0f, sz-1.0f));

    float u = fade(sx);
    float w = fade(sz);

    float nx0 = lerp(n00, n10, u);
    float nx1 = lerp(n01, n11, u);

    float nxz = lerp(nx0, nx1, w);

    return nxz;
}

float PerlinNoise::noise3d(float x, float y, float z) {
    float x0 = std::floor(x);
    float y0 = std::floor(y);
    float z0 = std::floor(z);

    int x1 = x0 + 1;
    int y1 = y0 + 1;
    int z1 = z0 + 1;

    float sx = x - x0;
    float sy = y - y0;
    float sz = z - z0;

    glm::vec3 g000 = grad3d(x0, y0, z0);
    glm::vec3 g100 = grad3d(x1, y0, z0);
    glm::vec3 g010 = grad3d(x0, y1, z0);
    glm::vec3 g110 = grad3d(x1, y1, z0);
    glm::vec3 g001 = grad3d(x0, y0, z1);
    glm::vec3 g101 = grad3d(x1, y0, z1);
    glm::vec3 g011 = grad3d(x0, y1, z1);
    glm::vec3 g111 = grad3d(x1, y1, z1);

    float n000 = glm::dot(g000, glm::vec3(sx, sy, sz));
    float n100 = glm::dot(g100, glm::vec3(sx-1.0f, sy, sz));
    float n010 = glm::dot(g010, glm::vec3(sx, sy-1.0f, sz));
    float n001 = glm::dot(g001, glm::vec3(sx, sy, sz-1.0f));
    float n110 = glm::dot(g110, glm::vec3(sx-1.0f, sy-1.0f, sz));
    float n011 = glm::dot(g011, glm::vec3(sx, sy-1.0f, sz-1.0f));
    float n101 = glm::dot(g101, glm::vec3(sx-1.0f, sy, sz-1.0f));
    float n111 = glm::dot(g111, glm::vec3(sx-1.0f, sy-1.0f, sz-1.0f));

    float u = fade(sx);
    float v = fade(sy);
    float w = fade(sz);

    float nx00 = lerp(n000, n100, u);
    float nx10 = lerp(n010, n110, u);
    float nx01 = lerp(n001, n101, u);
    float nx11 = lerp(n011, n111, u);

    float nxy0 = lerp(nx00, nx10, v);
    float nxy1 = lerp(nx01, nx11, v);

    float nxyz = lerp(nxy0, nxy1, w);

    return nxyz;
}

double PerlinNoise::fade(double t) {
    return 6 * std::pow(t, 5) - 15 * std::pow(t, 4) + 10 * std::pow(t, 3);
}

double PerlinNoise::lerp(double t, double a, double b) {
    return a + t * (b - a);
}

glm::vec2 PerlinNoise::grad2d(int x, int z) {
     int hash = perm[(perm[x & 255] + z) & 255];
     return grads2d[hash % 8];
}

glm::vec3 PerlinNoise::grad3d(int x, int y, int z) {
     int hash = perm[(perm[(perm[x & 255] + y) & 255] + z) & 255];
     return grads3d[hash % 12];
}
