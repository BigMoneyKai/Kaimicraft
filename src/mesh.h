#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cstddef>
#include <vector>

#include "block.h"
#include "texturemanager.h"
#include "util.h"
#include "tile.h"

constexpr int chunkLength = 16; // X
constexpr int chunkHeight = 24 * 16; // Y (384 blocks)
constexpr int chunkWidth = 16;  // Z

constexpr int genRadius = 180;

//                +-----------------------+
//               /|                      /|
//              / |                     / |
//             /  |                    /  |
//            /   |                   /   |
//           /    |                  /    |
//          /     |                 /     |
//         /      |                /      |
//        +----------+------------+       |
//        |       |  |            |       |
//        |       |  |            |       |
//        |       |  |            |       |
//        |       |  |            |       |
//        |       |  |heightScale |       |
//        |       |  |            |       |
//        |       +--|------------|-------+
//        |      /   .------------+      /
//        |     /     widthScale  |     /
//        |    /                  |    /
//        |   /                   |   /
//        |  /                    |  /
//        | /                     | /
//        |/                      |/
//        +-----------------------+

// meta block config:
constexpr glm::vec3 metaCenter  = {0.0f, 0.0f, 0.0f};
constexpr float widthScale      = 0.5f;
constexpr float heightScale     = 0.5f;


struct Vertex {
    glm::vec3 pos;
    glm::vec3 rgb;
    glm::vec2 uv;
};

constexpr Vertex metaVertices[] = {
    // 前面 (z = +0.5)
    {{-0.5f,-0.5f, 0.5f},{0,0,0},{0,0}},
    {{ 0.5f,-0.5f, 0.5f},{0,0,0},{1,0}},
    {{ 0.5f, 0.5f, 0.5f},{0,0,0},{1,1}},
    {{-0.5f, 0.5f, 0.5f},{0,0,0},{0,1}},

    // 后面 (z = -0.5)
    {{ 0.5f,-0.5f,-0.5f},{0,0,0},{0,0}},
    {{-0.5f,-0.5f,-0.5f},{0,0,0},{1,0}},
    {{-0.5f, 0.5f,-0.5f},{0,0,0},{1,1}},
    {{ 0.5f, 0.5f,-0.5f},{0,0,0},{0,1}},

    // 左面 (x = -0.5)
    {{-0.5f,-0.5f,-0.5f},{0,0,0},{0,0}},
    {{-0.5f,-0.5f, 0.5f},{0,0,0},{1,0}},
    {{-0.5f, 0.5f, 0.5f},{0,0,0},{1,1}},
    {{-0.5f, 0.5f,-0.5f},{0,0,0},{0,1}},

    // 右面 (x = +0.5)
    {{ 0.5f,-0.5f, 0.5f},{0,0,0},{0,0}},
    {{ 0.5f,-0.5f,-0.5f},{0,0,0},{1,0}},
    {{ 0.5f, 0.5f,-0.5f},{0,0,0},{1,1}},
    {{ 0.5f, 0.5f, 0.5f},{0,0,0},{0,1}},

    // 顶面 (y = +0.5)
    {{-0.5f, 0.5f, 0.5f},{0,0,0},{0,0}},
    {{ 0.5f, 0.5f, 0.5f},{0,0,0},{1,0}},
    {{ 0.5f, 0.5f,-0.5f},{0,0,0},{1,1}},
    {{-0.5f, 0.5f,-0.5f},{0,0,0},{0,1}},

    // 底面 (y = -0.5)
    {{-0.5f,-0.5f,-0.5f},{0,0,0},{0,0}},
    {{ 0.5f,-0.5f,-0.5f},{0,0,0},{1,0}},
    {{ 0.5f,-0.5f, 0.5f},{0,0,0},{1,1}},
    {{-0.5f,-0.5f, 0.5f},{0,0,0},{0,1}},
};

constexpr int metaIndices[] = {
     0, 1, 2,  0, 2, 3,   // 前
     4, 5, 6,  4, 6, 7,   // 后
     8, 9,10,  8,10,11,   // 左
    12,13,14, 12,14,15,   // 右
    16,17,18, 16,18,19,   // 顶
    20,21,22, 20,22,23,   // 底
};


class Mesh {
public:
    void init();
    void draw() const;
    void destroy();
    void updateBuffers();
    void buildFromBlocks(const BlockType* blocks, int sizeX, int sizeY, int sizeZ, const glm::ivec3& chunkCoord);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

private:
    void genChunkMeshBlocks();
};
