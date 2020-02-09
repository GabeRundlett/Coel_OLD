#include <Coel.hpp>
#include <cmath>

constexpr unsigned int QUAD_COUNT = 10, VERTEX_COUNT = QUAD_COUNT * 4, INDEX_COUNT = QUAD_COUNT * 6;
struct Vertex {
    float x, y;
};
struct Quad {
    Vertex a, b, c, d;
};

int main() {
    Coel::Window window(800, 600, "Simple Dynamic Index Buffer Example");

    Coel::Vbo vbo(nullptr, VERTEX_COUNT * sizeof(Vertex), {
        {Coel::Element::F32, 2},
    });

    Coel::Vao vao;
    vao.add(vbo);

    Coel::Ibo ibo(nullptr, INDEX_COUNT * sizeof(unsigned short));

    Quad *quads = nullptr;
    unsigned short *indices = nullptr;

    while (window.isOpen()) {
        Coel::Renderer::Command::clear();

        vbo.open(&quads);
        ibo.open(&indices);

        for (int i = 0; i < QUAD_COUNT; ++i) {
            float s = std::sinf(window.getTime() + 6.28 / QUAD_COUNT * i);
            quads[i] = {
                {-1.f / QUAD_COUNT + (2.f * i - QUAD_COUNT + 1.f) / QUAD_COUNT, -1.f / QUAD_COUNT + s},
                {1.f / QUAD_COUNT + (2.f * i - QUAD_COUNT + 1.f) / QUAD_COUNT, -1.f / QUAD_COUNT + s},
                {-1.f / QUAD_COUNT + (2.f * i - QUAD_COUNT + 1.f) / QUAD_COUNT, 1.f / QUAD_COUNT + s},
                {1.f / QUAD_COUNT + (2.f * i - QUAD_COUNT + 1.f) / QUAD_COUNT, 1.f / QUAD_COUNT + s},
            };

            const unsigned short offset = i * 4;

            *indices = offset + 0;
            ++indices;
            *indices = offset + 1;
            ++indices;
            *indices = offset + 2;
            ++indices;
            *indices = offset + 1;
            ++indices;
            *indices = offset + 3;
            ++indices;
            *indices = offset + 2;
            ++indices;
        }

        ibo.close();
        vbo.close();
        
        vao.drawIndexed(INDEX_COUNT);

        window.update();
    }
}
