#include <Coel.hpp>

int main() {
    Coel::Window window;
    Coel::create(window);

    Coel::Renderer::Batch2d batch;

    while (window.isOpen) {
        Coel::Renderer::clearColor();

        batch.begin();
        float time = Coel::getTime();
        for (unsigned int i = 0; i < 10; ++i) {
            float y = std::sin(0.5f * i + time) * 0.9;
            batch.submitRect(0.2 * i - 1, y - 0.1, 0.2, 0.2);
        }
        batch.flush();

        Coel::update(window);
    }

    batch.deinit();

    Coel::destroy(window);
}
