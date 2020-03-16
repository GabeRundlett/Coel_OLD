#include <Coel.hpp>

constexpr unsigned int QUAD_COUNT = 10, VERTEX_COUNT = QUAD_COUNT * 4, INDEX_COUNT = QUAD_COUNT * 6;
struct Vertex {
    float x, y;
};
struct QuadVertices {
    Vertex a, b, c, d;
};
struct QuadIndices {
    unsigned int a1, a2, a3, b1, b2, b3;
};

int main() {
    Coel::Window window(800, 600, "Simple Dynamic Index Buffer Example");
    Coel::Vbo vbo(nullptr, VERTEX_COUNT * sizeof(Vertex), {{Coel::Element::F32, 2}});
    Coel::Vao vao;
    vao.add(vbo);
    Coel::Ibo ibo(nullptr, INDEX_COUNT * sizeof(unsigned int));
    QuadVertices *quadVertices = nullptr;
    QuadIndices *quadIndices = nullptr;
    while (window.isOpen()) {
        Coel::Renderer::clear();
        vbo.open(&quadVertices);
        ibo.open(&quadIndices);
        for (int i = 0; i < QUAD_COUNT; ++i) {
            float s = (float)glm::sin(window.getTime() + 6.28 / QUAD_COUNT * i);
            quadVertices[i] = {
                {-1.f / QUAD_COUNT + (2.f * i - QUAD_COUNT + 1.f) / QUAD_COUNT, -1.f / QUAD_COUNT + s},
                {1.f / QUAD_COUNT + (2.f * i - QUAD_COUNT + 1.f) / QUAD_COUNT, -1.f / QUAD_COUNT + s},
                {-1.f / QUAD_COUNT + (2.f * i - QUAD_COUNT + 1.f) / QUAD_COUNT, 1.f / QUAD_COUNT + s},
                {1.f / QUAD_COUNT + (2.f * i - QUAD_COUNT + 1.f) / QUAD_COUNT, 1.f / QUAD_COUNT + s},
            };
            const unsigned int offset = i * 4;
            quadIndices[i] = {
                offset + 0, offset + 1, offset + 2, offset + 1, offset + 3, offset + 2,
            };
        }
        ibo.close();
        vbo.close();
        vao.drawIndexed(INDEX_COUNT);
        window.update();
    }
}
