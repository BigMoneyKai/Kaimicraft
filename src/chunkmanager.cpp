#include "chunkmanager.h"

#include <cmath>
#include <algorithm>

void ChunkManager::init(int cx, int cz) {
    m_chunkX = cx;
    m_chunkZ = cz;
}

void ChunkManager::buildMesh() {
     
}

static int worldToChunk(float v, int size) {
    return (int)std::floor(v / (float)size);
}

void ChunkManager::generateChunkBlocks(Chunk& chunk, TerrainGenerator* tgPtr) {
    for(int x = 0; x < chunkX; x++)
    for(int z = 0; z < chunkZ; z++) {
        int worldX = chunk.coord.x * chunkX + x;
        int worldZ = chunk.coord.z * chunkZ + z;
        
        int height = tgPtr->getHeight(worldX, worldZ);
         
        for(int y = 0; y < chunkY; y++) {
            int worldY = chunk.coord.y * chunkY + y;
            int randDirtLayer = static_cast<int>(Util::randomNum(3.0f, 6.0f)); 
            // Air 
            if(height < worldY) {
                chunk.blocks[x][y][z] = AIR;
            }

            // Surface(grass)
            else if(height - 1 < worldY && worldY <= height) {
                chunk.blocks[x][y][z] = GRASS;
            }

            // Dirt layer
            else if(height - randDirtLayer < worldY && worldY <= height - 1) {
                chunk.blocks[x][y][z] = DIRT;
            }

            // Rock layer
            else {
                chunk.blocks[x][y][z] = STONE;
            }
        }
    }

}

void ChunkManager::buildChunkMesh(Chunk& chunk) {
    if(!chunk.meshReady) {
        chunk.mesh.init();
        chunk.meshReady = true;
    }
    chunk.mesh.buildFromBlocks(&chunk.blocks[0][0][0], chunkX, chunkY, chunkZ,
                               glm::ivec3(chunk.coord.x, chunk.coord.y, chunk.coord.z));
}

void ChunkManager::updateDynamic(const glm::vec3& camPos, TerrainGenerator* tgPtr) {
    const int radiusChunks = std::max(1, genRadius / chunkX);
    ChunkCoord center{
        worldToChunk(camPos.x, chunkX),
        0, 
        worldToChunk(camPos.z, chunkZ)
    };

    if(m_hasCenter && center == m_center) {
        return;
    }
    m_hasCenter = true;
    m_center = center;

    std::unordered_set<ChunkCoord, ChunkCoordHash> wanted;
    wanted.reserve((2 * radiusChunks + 1) * (2 * radiusChunks + 1) * (2 * radiusChunks + 1));

    for(int x = center.x - radiusChunks; x <= center.x + radiusChunks; x++) {
        for(int y = minChunkY; y <= maxChunkY; y++) {
            for(int z = center.z - radiusChunks; z <= center.z + radiusChunks; z++) {
                wanted.insert({x, y, z});
            }
        }
    }

    for(const auto& coord : wanted) {
        if(m_chunks.find(coord) == m_chunks.end()) {
            Chunk chunk{};
            chunk.coord = coord;
            chunk.meshReady = false;
            generateChunkBlocks(chunk, tgPtr);
            buildChunkMesh(chunk);
            m_chunks.emplace(coord, std::move(chunk));
        }
    }

    for(auto it = m_chunks.begin(); it != m_chunks.end(); ) {
        if(wanted.find(it->first) == wanted.end()) {
            it->second.mesh.destroy();
            it = m_chunks.erase(it);
        } else {
            ++it;
        }
    }

    m_visibleMeshes.clear();
    m_visibleMeshes.reserve(m_chunks.size());
    for(auto& kv : m_chunks) {
        m_visibleMeshes.push_back(&kv.second.mesh);
    }
}

void ChunkManager::destroy() {

}
