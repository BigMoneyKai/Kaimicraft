#pragma once

#include "perlinnoise.h"

class TerrainGenerator {
public:
    void init();
    int getHeight(int x, int z);
    float getDensity(int x, int y, int z);

    void destroy();
private:
    float fbm2d(float x, float z);
private:
    PerlinNoise m_perlinNoise;

    int m_octaves;
    float m_lacunarity;
    float m_gain;

    float m_continentFreq;
    float m_ridgeFreq;
    float m_erosionFreq;
    float m_detailFreq;
};
