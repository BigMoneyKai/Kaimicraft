#pragma once 

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <glm/glm.hpp>

#include "block.h"
#include "terraingenerator.h"
#include "mesh.h"
#include "util.h"

constexpr int chunkX = 16;
constexpr int chunkY = 16;
constexpr int chunkZ = 16;

constexpr int genRadius = 64;

constexpr int verticalChunks = chunkY / chunkY; 
constexpr int minChunkY = 0;
constexpr int maxChunkY = 24;

class ChunkManager {
public:
   void init(int cx, int cz);

   void buildMesh();

   void updateDynamic(const glm::vec3& camPos, TerrainGenerator* tgPtr);
   const std::vector<Mesh*>& meshes() const { return m_visibleMeshes; }

   void destroy();

private:
    int m_chunkX;
    int m_chunkY;
    int m_chunkZ;

private:
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
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };

    struct Chunk {
        ChunkCoord coord;
        BlockType blocks[chunkX][chunkY][chunkZ];
        Mesh mesh;
        bool meshReady;
    };

    void generateChunkBlocks(Chunk& chunk, TerrainGenerator* tg);
    void buildChunkMesh(Chunk& chunk);

    std::unordered_map<ChunkCoord, Chunk, ChunkCoordHash> m_chunks;
    std::vector<Mesh*> m_visibleMeshes;
    bool m_hasCenter = false;
    ChunkCoord m_center{0, 0, 0};
};
