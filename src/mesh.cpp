#include "mesh.h"

void Mesh::init() {
   vao = vbo = ebo = 0;
   
   genChunkMeshBlocks();

   // VAO
   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);

   // VBO
   glGenBuffers(1, &vbo);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

   // EBO
   glGenBuffers(1, &ebo);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

   // Vertex layout
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, pos)
    );
   
   // Color
   glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, rgb)
    );
   // UV
   glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, uv)
    );

    glBindVertexArray(0);
}

void Mesh::draw() const {
   glBindVertexArray(vao); 
   glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
}

void Mesh::destroy() {
   if(vao != 0) glDeleteVertexArrays(1, &vao);
   if(vbo != 0) glDeleteBuffers(1, &vbo);
   if(ebo != 0) glDeleteBuffers(1, &ebo);

   vao = vbo = ebo = 0;
}

void Mesh::updateBuffers() {
    bool setupLayout = false;

    if(vao == 0) {
        glGenVertexArrays(1, &vao);
        setupLayout = true;
    }
    if(vbo == 0) {
        glGenBuffers(1, &vbo);
        setupLayout = true;
    }
    if(ebo == 0) {
        glGenBuffers(1, &ebo);
        setupLayout = true;
    }

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    if(setupLayout) {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, pos)
        );

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, rgb)
        );

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            2, 2, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, uv)
        );
    }

    glBindVertexArray(0);
}

// ─── 面枚举 ───────────────────────────────────────────────
enum Face { FRONT=0, BACK, LEFT, RIGHT, TOP, BOTTOM, FACE_COUNT };

static const glm::ivec3 faceNormals[FACE_COUNT] = {
    { 0,  0,  1},   // FRONT  +Z
    { 0,  0, -1},   // BACK   -Z
    {-1,  0,  0},   // LEFT   -X
    { 1,  0,  0},   // RIGHT  +X
    { 0,  1,  0},   // TOP    +Y
    { 0, -1,  0},   // BOTTOM -Y
};

// 顶点顺序：BL BR TR TL（正面朝外）
static const glm::vec3 faceVerts[FACE_COUNT][4] = {
    // FRONT  (z=+0.5)
    {
        {-0.5f,-0.5f, 0.5f},
        { 0.5f,-0.5f, 0.5f},
        { 0.5f, 0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f}
    },

    // BACK   (z=-0.5)
    {
        { 0.5f,-0.5f,-0.5f},
        {-0.5f,-0.5f,-0.5f},
        {-0.5f, 0.5f,-0.5f},
        {0.5f, 0.5f,-0.5f}
    },
    
    // LEFT   (x=-0.5)
    {
        {-0.5f,-0.5f,-0.5f},
        {-0.5f,-0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f},
        {-0.5f, 0.5f,-0.5f}
    },
    
    // RIGHT  (x=+0.5)
    {
        { 0.5f,-0.5f, 0.5f},
        { 0.5f,-0.5f,-0.5f},
        { 0.5f, 0.5f,-0.5f},
        { 0.5f, 0.5f, 0.5f}
    },
    
    // TOP    (y=+0.5)
    {
        {-0.5f, 0.5f, 0.5f},
        { 0.5f, 0.5f, 0.5f},
        { 0.5f, 0.5f,-0.5f},
        {-0.5f, 0.5f,-0.5f}
    },
    
    // BOTTOM (y=-0.5)
    {
        {-0.5f,-0.5f,-0.5f},
        { 0.5f,-0.5f,-0.5f},
        { 0.5f,-0.5f, 0.5f},
        {-0.5f,-0.5f, 0.5f}
    },
};

// tiles[0]=grass-top  tiles[1]=grass-side  tiles[2]=dirt
static const int faceTileIndex[FACE_COUNT] = {
    1,  // FRONT  → grass-side
    1,  // BACK   → grass-side
    1,  // LEFT   → grass-side
    1,  // RIGHT  → grass-side
    0,  // TOP    → grass-top
    2,  // BOTTOM → dirt
};

// ─── 压入一个面 ───────────────────────────────────────────
static void pushFace(
    std::vector<Vertex>& vertices,
    std::vector<unsigned int>& indices,
    unsigned int& indexOffset,
    Face face,
    const glm::vec3& blockPos,
    const glm::vec4& uv   // (u0, v0, u1, v1)
) {
    const glm::vec2 uvs[4] = {
        {uv.x, uv.y},  // BL
        {uv.z, uv.y},  // BR
        {uv.z, uv.w},  // TR
        {uv.x, uv.w},  // TL
    };
    for(int v = 0; v < 4; v++) {
        vertices.push_back({
            blockPos + faceVerts[face][v],
            {0.0f, 0.0f, 0.0f},
            uvs[v]
        });
    }
    indices.push_back(indexOffset + 0);
    indices.push_back(indexOffset + 1);
    indices.push_back(indexOffset + 2);
    indices.push_back(indexOffset + 0);
    indices.push_back(indexOffset + 2);
    indices.push_back(indexOffset + 3);
    indexOffset += 4;
}

// ─── 体素数据 [x][z][y]，1=实心 0=空气 ───────────────────
static int chunkData[chunkLength][chunkWidth][chunkHeight];

static bool isSolid(int x, int y, int z) {
    if(x < 0 || x >= chunkLength) return false;
    if(y < 0 || y >= chunkHeight) return false;
    if(z < 0 || z >= chunkWidth)  return false;
    return chunkData[x][z][y] != 0;
}

// ─── 生成区块网格 ─────────────────────────────────────────
void Mesh::genChunkMeshBlocks() {
    vertices.clear();
    indices.clear();

    // 全部填为草方块
    for(int x = 0; x < chunkLength; x++)
        for(int z = 0; z < chunkWidth; z++)
            for(int y = 0; y < chunkHeight; y++)
                chunkData[x][z][y] = 1;

    unsigned int indexOffset = 0;

    for(int x = 0; x < chunkLength; x++) {
        for(int z = 0; z < chunkWidth; z++) {
            for(int y = 0; y < chunkHeight; y++) {
                if(!isSolid(x, y, z)) continue;

                const glm::vec3 blockPos = {(float)x, (float)y, (float)z};

                for(int f = 0; f < FACE_COUNT; f++) {
                    const glm::ivec3& n = faceNormals[f];
                    // 邻居是实心 → 该面被遮挡，跳过（面剔除优化）
                    if(isSolid(x + n.x, y + n.y, z + n.z)) continue;

                    const int tileIdx = faceTileIndex[f];
                    const glm::vec4 uv = Util::calcTileUV(
                        (int)tiles[tileIdx].x,
                        (int)tiles[tileIdx].y,
                        stride, padding,
                        atlasWidth, atlasHeight
                    );
                    pushFace(vertices, indices, indexOffset,
                             static_cast<Face>(f), blockPos, uv);
                }
            }
        }
    }
}

static int tileIndexForBlock(BlockType type, Face face) {
    switch(type) {
        case GRASS:
            if(face == TOP) return 0;
            if(face == BOTTOM) return 2;
            return 1;
        case DIRT:
            return 2;
        case STONE:
            return 3;
        default:
            return 0;
    }
}

void Mesh::buildFromBlocks(const BlockType* blocks, int sizeX, int sizeY, int sizeZ, const glm::ivec3& chunkCoord) {
    vertices.clear();
    indices.clear();

    auto indexOf = [sizeY, sizeZ](int x, int y, int z) {
        return (x * sizeY + y) * sizeZ + z;
    };

    auto isSolidBlock = [&](int x, int y, int z) {
        if(x < 0 || x >= sizeX) return false;
        if(y < 0 || y >= sizeY) return false;
        if(z < 0 || z >= sizeZ) return false;
        return blocks[indexOf(x, y, z)] != AIR;
    };

    unsigned int indexOffset = 0;

    for(int x = 0; x < sizeX; x++) {
        for(int z = 0; z < sizeZ; z++) {
            for(int y = 0; y < sizeY; y++) {
                const BlockType blockType = blocks[indexOf(x, y, z)];
                if(blockType == AIR) continue;

                const glm::vec3 blockPos = {
                    (float)(chunkCoord.x * sizeX + x),
                    (float)(chunkCoord.y * sizeY + y),
                    (float)(chunkCoord.z * sizeZ + z)
                };

                for(int f = 0; f < FACE_COUNT; f++) {
                    const glm::ivec3& n = faceNormals[f];
                    if(isSolidBlock(x + n.x, y + n.y, z + n.z)) continue;

                    const int tileIdx = tileIndexForBlock(blockType, static_cast<Face>(f));
                    const glm::vec4 uv = Util::calcTileUV(
                        (int)tiles[tileIdx].x,
                        (int)tiles[tileIdx].y,
                        stride, padding,
                        atlasWidth, atlasHeight
                    );
                    pushFace(vertices, indices, indexOffset,
                             static_cast<Face>(f), blockPos, uv);
                }
            }
        }
    }

    updateBuffers();
}
