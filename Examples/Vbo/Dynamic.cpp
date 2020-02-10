#include <Coel.hpp>
#include <cmath>

constexpr unsigned int TRIANGLE_COUNT = 10, VERTEX_COUNT = TRIANGLE_COUNT * 3;
struct Vertex {
    float x, y;
};
struct Triangle {
    Vertex a, b, c;
};

int main() {
    Coel::Window window(800, 600, "Simple Dynamic Vertex Buffer Example");

    Triangle *triangles = nullptr;
    Coel::Vbo vbo(nullptr, VERTEX_COUNT * sizeof(Vertex), {{Coel::Element::F32, 2}});
    Coel::Vao vao;
    vao.add(vbo);

    while (window.isOpen()) {
        Coel::Renderer::Command::clear();

        vbo.open(&triangles);
        for (int i = 0; i < TRIANGLE_COUNT; ++i) {
            float s = (float)std::sin(window.getTime() + 6.28 / TRIANGLE_COUNT * i);
            triangles[i] = {
                {-1.f / TRIANGLE_COUNT + (2.f * i - 9.f) / TRIANGLE_COUNT, -1.f / TRIANGLE_COUNT + s},
                {1.f / TRIANGLE_COUNT + (2.f * i - 9.f) / TRIANGLE_COUNT, -1.f / TRIANGLE_COUNT + s},
                {0.f + (2.f * i - 9.f) / TRIANGLE_COUNT, 1.f / TRIANGLE_COUNT + s},
            };
        }
        vbo.close();

        vao.draw(TRIANGLE_COUNT * 3);

        window.update();
    }
}
