#include <Coel.hpp>

static Coel::Window window(800, 600, "Window");
static Coel::Renderer::BatchText textRenderer;
static Coel::Renderer::Batch2d renderer;

int main() {
    window.onResize = [](Coel::Window &w) {
        textRenderer.resize(w.size); //
    };
    window.onResize(window);

    Coel::Renderer::setClearColor(0.1f, 0.1f, 0.1f, 1.f);

    textRenderer.fill({0x666666ff});
    // renderer.fill(0x313131ff);

    while (window.isOpen()) {
        Coel::Renderer::clear();

        // renderer.submitRect(0, 0, 1, 1);
        // renderer.flush();

        float s = 3;
        textRenderer.submitText(0, s / 10 * 0000, s, " !\"#$%&'()*+,-./");
        textRenderer.submitText(0, s / 10 * 1024, s, "0123456789:;<=>?@");
        textRenderer.submitText(0, s / 10 * 2048, s, "ABCDEFGHIJKLMNOPQ");
        textRenderer.submitText(0, s / 10 * 3072, s, "RSTUVWXYZ[\\]^_`");
        textRenderer.submitText(0, s / 10 * 4096, s, "abcdefghijklmnopqr");
        textRenderer.submitText(0, s / 10 * 5120, s, "rstuvwxyz{|}~@");
        textRenderer.flush();

        window.update();
    }
}
