#pragma once

#include "perlinnoise.h"

class TerrainGenerator {
public:
    void init();
    int getHeight(int x, int z);
    float getDensity(int x, int y, int z);

    void destroy();
private:

private:
    PerlinNoise m_perlinNoise;
    
    int m_octaves;
    float m_lacunarity;
    float m_gain;
};
