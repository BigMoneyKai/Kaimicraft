#pragma once

#include <glm/glm.hpp>

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <cmath>
#include <memory>

#include "block.h"
#include "terraingenerator.h"
#include "mesh.h"
#include "util.h"
#include "threadpool.h"

constexpr int baseHeight = -64;

struct ChunkCoord {
    int x;
    int y;
    int z;

    bool operator==(const ChunkCoord& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct ChunkCoordHash {
    std::size_t operator()(const ChunkCoord& c) const {
        std::size_t h1 = std::hash<int>{}(c.x);
        std::size_t h2 = std::hash<int>{}(c.y);
        std::size_t h3 = std::hash<int>{}(c.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h1 << 7);
    }
};

struct Chunk {
    ChunkCoord coord;
    BlockType blocks[chunkLength][chunkHeight][chunkWidth];
    std::vector<Vertex> vertices;

    Mesh mesh;
    bool blockReady = false;
    bool meshReady = false;
    bool uploaded = false;
};

class ChunkManager {
public:
    void init();

    void updateDynamic(const glm::vec3& camPos, TerrainGenerator* tgPtr, ThreadPool& threadPool);
    std::vector<const Mesh*> meshes();

    void destroy();

private:
    void generateChunkBlocks(Chunk& chunk, TerrainGenerator* tgPtr);
    std::unordered_set<ChunkCoord, ChunkCoordHash> computeWantedChunks(const glm::vec3& camPos);
private:
    std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, ChunkCoordHash> m_chunks;

    int m_grassLayerDepth;
    int m_dirtLayerDepth;
    int m_bedrockLayerDepth;
};
