#include <Coel.hpp>
#include <cmath>

int main() {
    constexpr unsigned int QUAD_COUNT = 10;

    Coel::Window window(800, 600, "Simple Batch Rendering Example");
    Coel::Renderer::Batch2d renderer(10000, 10000);

    Coel::Renderer::Command::setClearColor(0.2f, 0.4f, 0.8f, 1.f);

    while (window.isOpen()) {
        Coel::Renderer::Command::clear();

        for (int i = 0; i < QUAD_COUNT; ++i) {
            float s = std::sinf(window.getTime() + 6.28 / QUAD_COUNT * i);
            renderer.submitRect(2.f / QUAD_COUNT * i - 1, s, 2.f / QUAD_COUNT, 2.f / QUAD_COUNT);
        }

        renderer.flush();
        window.update();
    }
}
