#include "chunkmanager.h"

#include <cmath>
#include <algorithm>

static int worldToChunk(float v, int size) {
    return (int)std::floor(v / (float)size);
}

static std::vector<std::pair<int, int>> generateRingOffsets(int outerR, int innerR = 0) {
    std::vector<std::pair<int, int>> offsets;
    for (int dz = -outerR; dz <= outerR; ++dz) {
        int dz2 = dz * dz;
        if (dz2 > outerR * outerR) continue;

        // 外半径允许的最大 |dx|
        int dxMaxOuter = static_cast<int>(std::sqrt(outerR * outerR - dz2));
        // 内半径允许的最大 |dx|（环内不包含内半径以内的点）
        int dxMaxInner = -1; // 默认无内限制
        if (dz2 <= innerR * innerR) {
            // 内半径内需要排除的点：|dx| <= floor(sqrt(innerR^2 - dz2))
            dxMaxInner = static_cast<int>(std::sqrt(innerR * innerR - dz2));
        }

        // 输出左半环 (dx 从 -dxMaxOuter 到 -dxMaxInner-1)
        for (int dx = -dxMaxOuter; dx <= -dxMaxInner - 1; ++dx) {
            offsets.emplace_back(dx, dz);
        }
        // 输出右半环 (dx 从 dxMaxInner+1 到 dxMaxOuter)
        for (int dx = dxMaxInner + 1; dx <= dxMaxOuter; ++dx) {
            offsets.emplace_back(dx, dz);
        }
    }
    return offsets;
}

void ChunkManager::init() {
    m_grassLayerDepth = 1;
    m_dirtLayerDepth = static_cast<int>(Util::randomNum(3.0f, 6.0f));
    m_bedrockLayerDepth = 0;

}

void ChunkManager::generateChunkBlocks(Chunk& chunk, TerrainGenerator* tgPtr) {
    for(int x = 0; x < chunkLength; x++)
    for(int z = 0; z < chunkWidth; z++) {
        int worldX = chunk.coord.x * chunkLength + x;
        int worldZ = chunk.coord.z * chunkWidth + z;

        int height = tgPtr->getHeight(worldX, worldZ);

        for(int y = 0; y < chunkHeight; y++) {

            // Air
            if(height < y) {
                chunk.blocks[x][y][z] = AIR;
            }

            // Surface(grass)
            else if(height - 1 < y && y <= height) {
                chunk.blocks[x][y][z] = GRASS;
            }

            // Dirt layer
            else if(height - m_dirtLayerDepth < y && y <= height - 1) {
                chunk.blocks[x][y][z] = DIRT;
            }

            // Rock layer
            else {
                chunk.blocks[x][y][z] = STONE;
            }

            // update all layer depths
            // m_grassLayerDepth is always equal to 1
            m_dirtLayerDepth = static_cast<int>(Util::randomNum(3.0f, 6.0f));
        }
    }
}

void ChunkManager::updateDynamic(const glm::vec3& camPos, TerrainGenerator* tgPtr, ThreadPool& threadPool) {
    // Match original dynamic loading behavior:
    // - wanted set is a square area around the camera's center chunk (not a ring)
    // - unload chunks that fall outside the wanted set
    // Note: we keep everything else intact (async block generation + main-thread mesh build).

    // Compute center chunk (XZ only)
    ChunkCoord center;
    center.x = worldToChunk(camPos.x, chunkLength);
    center.z = worldToChunk(camPos.z, chunkWidth);
    center.y = 0;

    // Square radius in chunks (original used max(1, genRadius / chunkX))
    const int radiusChunks = std::max(1, genRadius / chunkLength);

    // Build wanted set (square area)
    std::unordered_set<ChunkCoord, ChunkCoordHash> wantedChunks;
    wantedChunks.reserve((2 * radiusChunks + 1) * (2 * radiusChunks + 1));

    for (int x = center.x - radiusChunks; x <= center.x + radiusChunks; ++x) {
        for (int z = center.z - radiusChunks; z <= center.z + radiusChunks; ++z) {
            wantedChunks.insert(ChunkCoord{x, 0, z});
        }
    }

    // 1) Ensure wanted chunks exist and start async block generation
    for (const auto& coord : wantedChunks) {
        auto it = m_chunks.find(coord);
        if (it == m_chunks.end()) {
            auto chunkPtr = std::make_unique<Chunk>();
            chunkPtr->coord = coord;

            // Insert into the member map
            auto [iter, inserted] = m_chunks.emplace(coord, std::move(chunkPtr));
            if (inserted) {
                Chunk* ptr = iter->second.get();
                // Generate blocks in worker thread (CPU-side only)
                threadPool.enqueueTask([this, ptr, tgPtr] {
                    generateChunkBlocks(*ptr, tgPtr);
                    ptr->blockReady = true;
                });
            }
        }
    }

    // 2) Unload chunks that are no longer wanted (match original behavior)
    for (auto it = m_chunks.begin(); it != m_chunks.end(); ) {
        if (wantedChunks.find(it->first) == wantedChunks.end()) {
            it->second->mesh.destroy();
            it = m_chunks.erase(it);
        } else {
            ++it;
        }
    }

    // 3) On main thread: build mesh + upload to GPU when blocks are ready
    //    (OpenGL calls must stay on the thread that owns the context)
    for (auto& [coord, chunkPtr] : m_chunks) {
        if (chunkPtr->blockReady && !chunkPtr->meshReady) {
            // Flatten [x][y][z] into a contiguous buffer matching Mesh::buildFromBlocks layout
            // indexOf in Mesh::buildFromBlocks: (x * sizeY + y) * sizeZ + z
            std::vector<BlockType> flat;
            flat.resize(chunkLength * chunkHeight * chunkWidth);

            auto indexOf = [](int x, int y, int z) {
                return (x * chunkHeight + y) * chunkWidth + z;
            };
            for (int x = 0; x < chunkLength; x++) {
                for (int y = 0; y < chunkHeight; y++) {
                    for (int z = 0; z < chunkWidth; z++) {
                        flat[indexOf(x, y, z)] = chunkPtr->blocks[x][y][z];
                    }
                }
            }

            chunkPtr->mesh.buildFromBlocks(
                flat.data(),
                chunkLength, chunkHeight, chunkWidth,
                glm::ivec3(coord.x, coord.y, coord.z)
            );

            chunkPtr->meshReady = true;
            chunkPtr->uploaded = true;
        }
    }
}

std::unordered_set<ChunkCoord, ChunkCoordHash>
ChunkManager::computeWantedChunks(const glm::vec3& camPos) {
    std::unordered_set<ChunkCoord, ChunkCoordHash> wanted;

    // 玩家所在区块坐标（仅考虑水平方向）
    ChunkCoord center;
    center.x = worldToChunk(camPos.x, chunkLength);
    center.z = worldToChunk(camPos.z, chunkWidth);
    center.y = 0;  // 如果有多层高度，可在此扩展

    // 生成厚度为1的圆环（外半径 = genRadius，内半径 = genRadius - 1）
    // 如果 genRadius <= 0，则返回空集
    if (genRadius <= 0) return wanted;

    int outerR = genRadius;
    int innerR = std::max(0, genRadius - 1);  // 厚度为1
    auto offsets = generateRingOffsets(outerR, innerR);

    for (const auto& [dx, dz] : offsets) {
        ChunkCoord coord;
        coord.x = center.x + dx;
        coord.z = center.z + dz;
        coord.y = 0; // 根据需要调整
        wanted.insert(coord);
    }

    // 如果希望同时包含玩家所在区块（中心点），可以单独添加
    wanted.insert(center);

    return wanted;
}

std::vector<const Mesh*> ChunkManager::meshes() {
    std::vector<const Mesh*> meshVec;
    meshVec.clear();
    meshVec.reserve(m_chunks.size());
    for(auto it = m_chunks.cbegin(); it != m_chunks.cend(); ++it) {
        meshVec.push_back(&it->second->mesh);
    }
    return meshVec;
}

void ChunkManager::destroy() {
    m_chunks.clear();
}
