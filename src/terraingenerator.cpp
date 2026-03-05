#include "terraingenerator.h"

void TerrainGenerator::init() {
    m_octaves = 6;
    m_lacunarity = 2.0f;
    m_gain = 0.5f;
}

int TerrainGenerator::getHeight(int x, int z) {
    float sum  = 0.0f;
    float freq = 0.01f;
    float ampl = 1.0f;

    float maxAmpl = 0.0f;

    for(int i = 0; i < m_octaves; i++) {
        sum += ampl * m_perlinNoise.noise2D(
                (float)x * freq, 
                (float)z * freq);
        
        maxAmpl += ampl;
        ampl    *= m_gain;
        freq    *= m_lacunarity;
    }
    float n = sum / maxAmpl;
    
    constexpr int baseHeight = 64;
    constexpr int heightScale = 32;
    
    return (int)(baseHeight + n * heightScale);
}

float TerrainGenerator::getDensity(int x, int y, int z) {
    float sum  = 0.0f;
    float freq = 1.0f;
    float ampl = 1.0f;

    float maxAmpl = 0.0f;

    for(int i = 0; i < m_octaves; i++) {
        sum += ampl * m_perlinNoise.noise3D(
                (float)x * freq, 
                (float)y * freq, 
                (float)z * freq);
        
        maxAmpl += ampl;
        ampl    *= m_gain;
        freq    *= m_lacunarity;
    }

    return sum / maxAmpl; 
}

void TerrainGenerator::destroy() {

}
