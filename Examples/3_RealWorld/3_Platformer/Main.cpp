#include <Coel.hpp>
#include <array>
#include <iostream>

Coel::Window window(1280, 900, "Platformer");

namespace Renderer {
    struct Vertex {
        glm::vec2 pos, tex;
    };
    constexpr unsigned int vCount = 10000 * 4, iCount = 10000 * 6;
    Coel::Vao vao{};
    Coel::Vbo vbo(nullptr, sizeof(Vertex) * vCount, {{Coel::Element::F32, 2}, {Coel::Element::F32, 2}});
    Coel::Ibo ibo(nullptr, sizeof(unsigned int) * iCount);
    Vertex *vertices{};
    unsigned int *indices{};
    unsigned int vertexCount{}, indexCount{};
    constexpr const char *const vertSrc = R"(
#version 440
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
out vec2 v_tex;
void main() {
    v_tex = tex;
    gl_Position = vec4((pos * 2 - 1) / vec2(1280, 900) * 32, 0, 1);
})";
    constexpr const char *const fragSrc = R"(
#version 440
in vec2 v_tex;
uniform sampler2D u_tex;
out vec4 color;
void main() {
    color = texture(u_tex, v_tex);
})";
    Coel::Shader shader(vertSrc, fragSrc);
    Coel::Texture texture("Assets/dirt.png");
    void init() {
        vao.add(vbo);
        texture.setMagFilter(Coel::Filter::Nearest);
    }
    void flush() {
        shader.bind();
        ibo.bind();
        texture.bind(0);
        vao.drawIndexed(indexCount);
    }
    void begin() {
        vertexCount = 0, indexCount = 0;
        vbo.open(&vertices);
        ibo.open(&indices);
    }
    void end() {
        vbo.close();
        ibo.close();
    }
    void submit(float x, float y) {
        struct QuadVbo {
            std::array<Vertex, 4> data;
        };
        struct QuadIbo {
            std::array<unsigned int, 6> data;
        };
        if (indexCount + 6 > iCount - 1) {
            end();
            flush();
            begin();
        }
        *(QuadVbo *)vertices = {{{
            {{0.f + x, 0.f + y}, {0.f, 0.f}},
            {{0.f + x, 1.f + y}, {0.f, 1.f}},
            {{1.f + x, 0.f + y}, {1.f, 0.f}},
            {{1.f + x, 1.f + y}, {1.f, 1.f}},
        }}};
        vertices += 4, vertexCount += 4;
        *(QuadIbo *)indices = {{{
            vertexCount + 0,
            vertexCount + 1,
            vertexCount + 2,
            vertexCount + 1,
            vertexCount + 3,
            vertexCount + 2,
        }}};
        indices += 6, indexCount += 6;
    }
} // namespace Renderer

struct Chunk {
    glm::vec2 pos;
    struct Element {
        unsigned char id;
    };
    static inline constexpr auto SIZE_X = 16, SIZE_Y = 16;
    std::array<std::array<Element, SIZE_Y>, SIZE_X> data{};
    Chunk(glm::vec2 p) : pos(p) {
        for (auto &row : data)
            for (auto &elem : row)
                elem.id = 1;
    }
    void draw() const {
        for (auto yi = 0; yi < SIZE_Y; ++yi) {
            for (auto xi = 0; xi < SIZE_X; ++xi)
                if (data[yi][xi].id > 0) Renderer::submit(pos.x * SIZE_X + xi, pos.y * SIZE_Y + yi);
        }
    }
};

std::vector<Chunk> chunks;

int main() {
    chunks.push_back({{-1, -1}});
    chunks.push_back({{-1, 0}});
    chunks.push_back({{-1, 1}});
    chunks.push_back({{0, -1}});
    chunks.push_back({{0, 0}});
    chunks.push_back({{0, 1}});
    chunks.push_back({{1, -1}});
    chunks.push_back({{1, 0}});
    chunks.push_back({{1, 1}});

    Renderer::init();
    Coel::Renderer::setClearColor(0.7, 0.72, 0.95, 1);
    while (window.isOpen()) {
        Coel::Renderer::clearColor();
        Renderer::begin();
        for (const auto &chunk : chunks)
            chunk.draw();
        Renderer::end();
        Renderer::flush();
        window.update();
    }
}
