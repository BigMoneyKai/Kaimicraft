#include "terraingenerator.h"

void TerrainGenerator::init() {
    m_octaves = 4;
    m_lacunarity = 2.0f;
    m_gain = 0.5f;

    m_continentFreq = 0.0015f;
    m_ridgeFreq = 0.008f;
    m_erosionFreq = 0.01f;
    m_detailFreq = 0.04f;
}

int TerrainGenerator::getHeight(int x, int z) {
    float continental = fbm2d(x * m_continentFreq, z * m_continentFreq);
    float ridge = fbm2d(x * m_ridgeFreq, z * m_ridgeFreq);
    float erosion = fbm2d(x * m_erosionFreq, z * m_erosionFreq);
    float detail = fbm2d(x * m_detailFreq, z * m_detailFreq);

    ridge = 1.0f - std::abs(ridge);
    erosion = (erosion + 1.0f) * 0.5f;

    // 组合高度
    float height =
        continental * 0.7f +
        ridge * (1.0f - erosion) * 0.25f +
        detail * 0.05f;

    height = (height + 1.0f) * 0.5f;

    // 平滑山体
    height = height * height * (3.0f - 2.0f * height);

    constexpr int heightScale = 100;

    return static_cast<int>(height * heightScale);
}

float TerrainGenerator::getDensity(int x, int y, int z) {
    float sum  = 0.0f;
    float freq = 1.0f;
    float ampl = 1.0f;

    float maxAmpl = 0.0f;

    for(int i = 0; i < m_octaves; i++) {
        sum += ampl * m_perlinNoise.noise3d(
                (float)x * freq,
                (float)y * freq,
                (float)z * freq);

        maxAmpl += ampl;
        ampl    *= m_gain;
        freq    *= m_lacunarity;
    }

    return sum / maxAmpl;
}

float TerrainGenerator::fbm2d(float x, float z) {
    float sum  = 0.0f;
    float ampl = 1.0f;
    float maxAmpl = 0.0f;

    for(int i = 0; i < m_octaves; i++) {
        sum += ampl * m_perlinNoise.noise2d(
                static_cast<float>(x),
                static_cast<float>(z));

        maxAmpl += ampl;
        ampl    *= m_gain;
    }
    return sum / maxAmpl;
}

void TerrainGenerator::destroy() {

}
