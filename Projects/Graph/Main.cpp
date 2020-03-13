#include <Coel.hpp>

static Coel::Window window(1600, 960, "Graph");
static Coel::Renderer::Batch2d batch;

int main() {
    window.onResize = [](Coel::Window &w) { batch.resize({0, 0}, w.size); };
    window.resize();

    while (window.isOpen()) {
        Coel::Renderer::clearColor();
        window.update();
    }
}
