#pragma once

#include <Coel.hpp>
#include <array>

struct Vertex {
    glm::vec3 pos;
    glm::vec2 tex;
};

constexpr unsigned int MAX_QUAD_COUNT = 10000, MAX_VCOUNT = MAX_QUAD_COUNT * 4, MAX_ICOUNT = MAX_QUAD_COUNT * 6;
constexpr unsigned int MAX_VSIZE = 10000 * 4 * sizeof(Vertex), MAX_ISIZE = 10000 * 6 * sizeof(unsigned int);

Coel::Renderer::Batch batch({{Coel::F32, 3}, {Coel::F32, 2}});
Vertex *vertexHandle{};
unsigned int *indexHandle{};
unsigned int vertexCount{}, indexCount{};

void begin() {
    open(batch.vbo, &vertexHandle);
    open(batch.ibo, &indexHandle);
}

void flush() {
    Coel::close(batch.vbo);
    Coel::close(batch.ibo);
    Coel::Renderer::drawIndexed(batch.vao, indexCount);
    vertexCount = 0, indexCount = 0;
}

void submitRectXY(const float x, const float y, const float z, const float w, const float h) {
    if (vertexCount + 4 > MAX_VCOUNT || indexCount + 6 > MAX_ICOUNT) {
        flush();
        begin();
    }

    *(std::array<Vertex, 4> *)vertexHandle = {{
        {{x + 0, y + 0, z}, {0, 0}},
        {{x + 0, y + h, z}, {0, 1}},
        {{x + w, y + 0, z}, {1, 0}},
        {{x + w, y + h, z}, {1, 1}},
    }};

    *(std::array<unsigned int, 6> *)indexHandle = {
        vertexCount + 0, //
        vertexCount + 1, //
        vertexCount + 2, //
        vertexCount + 1, //
        vertexCount + 3, //
        vertexCount + 2, //
    };

    vertexHandle += 4, indexHandle += 6;
    vertexCount += 4, indexCount += 6;
}

void submitRectXZ(const float x, const float y, const float z, const float w, const float h) {
    if (vertexCount + 4 > MAX_VCOUNT || indexCount + 6 > MAX_ICOUNT) {
        flush();
        begin();
    }

    *(std::array<Vertex, 4> *)vertexHandle = {{
        {{x + 0, y, z + 0}, {0, 0}},
        {{x + 0, y, z + h}, {0, 1}},
        {{x + w, y, z + 0}, {1, 0}},
        {{x + w, y, z + h}, {1, 1}},
    }};

    *(std::array<unsigned int, 6> *)indexHandle = {
        vertexCount + 0, //
        vertexCount + 1, //
        vertexCount + 2, //
        vertexCount + 1, //
        vertexCount + 3, //
        vertexCount + 2, //
    };

    vertexHandle += 4, indexHandle += 6;
    vertexCount += 4, indexCount += 6;
}

void submitRectZY(const float x, const float y, const float z, const float w, const float h) {
    if (vertexCount + 4 > MAX_VCOUNT || indexCount + 6 > MAX_ICOUNT) {
        flush();
        begin();
    }

    *(std::array<Vertex, 4> *)vertexHandle = {{
        {{x, y + 0, z + 0}, {0, 0}},
        {{x, y + h, z + 0}, {0, 1}},
        {{x, y + 0, z + w}, {1, 0}},
        {{x, y + h, z + w}, {1, 1}},
    }};

    *(std::array<unsigned int, 6> *)indexHandle = {
        vertexCount + 0, //
        vertexCount + 1, //
        vertexCount + 2, //
        vertexCount + 1, //
        vertexCount + 3, //
        vertexCount + 2, //
    };

    vertexHandle += 4, indexHandle += 6;
    vertexCount += 4, indexCount += 6;
}

void submitCube(const glm::vec3 &pos) {
    submitRectXY(pos.x + 0.5, pos.y + -0.5, pos.z + 0.5, -1, 1);  // front
    submitRectXY(pos.x + -0.5, pos.y + -0.5, pos.z + -0.5, 1, 1); // back
    submitRectXZ(pos.x + -0.5, pos.y + 0.5, pos.z + -0.5, 1, 1);  // top
    submitRectXZ(pos.x + 0.5, pos.y + -0.5, pos.z + -0.5, -1, 1); // bottom
    submitRectZY(pos.x + -0.5, pos.y + -0.5, pos.z + 0.5, -1, 1); // left
    submitRectZY(pos.x + 0.5, pos.y + -0.5, pos.z + -0.5, 1, 1);  // right
}
