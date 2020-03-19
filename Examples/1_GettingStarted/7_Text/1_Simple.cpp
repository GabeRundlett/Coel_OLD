#include <Coel.hpp>

int main() {
    Coel::Window window(1400, 800, "Text Rendering Example");

    Coel::Renderer::BatchText textBatch;
    textBatch.fill({112, 112, 112, 255});

    Coel::Renderer::setClearColor(0.1, 0.1, 0.1, 1);
    Coel::Renderer::enableBlend(true);

    while (window.isOpen()) {
        Coel::Renderer::clearColor();

        textBatch.resize({0, 0}, window.size);

        textBatch.begin();
        textBatch.submitText(0, 0, 4, "Hello, Text!");

        textBatch.flush();

        window.update();
    }
}
