#include <Coel.hpp>
#include <cmath>

int main() {
    constexpr unsigned int QUAD_COUNT = 10;

    Coel::Window window(800, 600, "Simple Batch Rendering Example");
    Coel::Renderer::Batch2d batch;

    Coel::Renderer::setClearColor(0.2f, 0.4f, 0.8f, 1.f);
    batch.fill({1, 1, 1, 1});
    batch.resize({-1, -1}, {2, 2});

    while (window.isOpen()) {
        Coel::Renderer::clearColor();

        for (int i = 0; i < QUAD_COUNT; ++i) {
            float s = (float)std::sin(window.getTime() + 6.28 / QUAD_COUNT * i);
            batch.submitRect(2.f / QUAD_COUNT * i - 1, s, 2.f / QUAD_COUNT, 2.f / QUAD_COUNT);
        }

        batch.flush();
        window.update();
    }
}
