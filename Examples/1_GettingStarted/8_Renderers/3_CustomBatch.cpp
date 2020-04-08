#include <Coel.hpp>
#include <array>

static constexpr const char *const vertSrc = R"(
#version 440
layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_tex;
void main() {
	gl_Position = vec4(a_pos, 0, 1);
}
)";

static constexpr const char *const fragSrc = R"(
#version 440
out vec4 color;
void main() {
    color = vec4(1, 0, 0, 1);
}
)";

struct Vertex {
    float x, y, w, h;
};

int main() {
    Coel::Window window;
    Coel::create(window);

    constexpr unsigned int MAX_SPRITE_COUNT = 10000, MAX_VCOUNT = MAX_SPRITE_COUNT * 4, MAX_ICOUNT = MAX_SPRITE_COUNT * 6,
                           MAX_VSIZE = 10000 * 4 * sizeof(Vertex), MAX_ISIZE = 10000 * 6 * sizeof(unsigned int);

    Coel::Renderer::Batch batch({{Coel::F32, 2}, {Coel::F32, 2}}, vertSrc, fragSrc);
    Coel::Renderer::init(batch, MAX_VSIZE, MAX_ISIZE);

    Vertex *vertexHandle;
    unsigned int *indexHandle;
    unsigned int vertexCount{}, indexCount{};

    const auto flush = [&batch, &vertexCount, &indexCount]() {
        Coel::close(batch.vbo);
        Coel::close(batch.ibo);
        Coel::Renderer::drawIndexed(batch.vao, indexCount);
        vertexCount = 0, indexCount = 0;
    };

    while (window.isOpen) {
        Coel::Renderer::clearColor();

        Coel::bind(batch.vao);
        batch.shader.bind();
        Coel::open(batch.vbo, &vertexHandle);
        Coel::open(batch.ibo, &indexHandle);

        float time = Coel::getTime();

        for (unsigned int i = 0; i < 10; ++i) {
            if (vertexCount + 4 > MAX_VCOUNT || indexCount + 6 > MAX_ICOUNT) flush();

            float x = 0.2 * i - 1;
            float y = std::sin(0.5f * i + time) * 0.9 - 0.1;
            float w = 0.2, h = 0.2;

            *(std::array<Vertex, 4> *)vertexHandle = {{
                {x, y, 0, 0},
                {x, y + h, 0, 1},
                {x + w, y, 1, 0},
                {x + w, y + h, 1, 1},
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

        flush();

        Coel::update(window);
    }

    Coel::Renderer::deinit(batch);
    Coel::destroy(window);
}
