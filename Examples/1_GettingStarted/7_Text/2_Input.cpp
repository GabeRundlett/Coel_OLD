#include <Coel.hpp>

std::string text = "Hello, Text!";
glm::vec2 textPos{100, 100};

int main() {
    Coel::Window window(1400, 800, "Text Input Example");
    window.onKey = [](Coel::Window &w) {
        auto keyInput = [](const Coel::KeyInfo &key) {
            if (key.code == Coel::Key::Backspace) {
                std::size_t pos = text.size() - 1;
                if (key.mods & Coel::Mod::Control) pos = text.rfind(' ');
                text = text.substr(0, pos);
            }
            if (key.code >= Coel::Key::A && key.code <= Coel::Key::Z)
                text.push_back(key.code + (key.mods & Coel::Mod::Shift ? 0 : 'a' - 'A'));
        };

        switch (w.key.action) {
        case Coel::Action::Press: keyInput(w.key); break;
        case Coel::Action::Repeat: keyInput(w.key); break;
        default: break;
        }
    };

    Coel::Renderer::BatchText textBatch;
    textBatch.resize({0, 0}, window.size);
    textBatch.fill({112, 112, 112, 255});

    Coel::Renderer::Batch2d batch;
    batch.resize({0, 0}, window.size);
    batch.fill({112, 112, 112, 255});

    Coel::Renderer::enableBlend(true);
    Coel::Renderer::setClearColor(0.1, 0.1, 0.1, 1);

    while (window.isOpen()) {
        Coel::Renderer::clearColor();

        float scale = 2.f;

        batch.begin();
        batch.fill({32, 32, 32, 255});
        batch.submitRect(textPos.x - 4, textPos.y - 4, window.size.x - (textPos.x + 4) * 2, 28 * 4 * scale + 8);
        batch.flush();

        Coel::Renderer::setClearColor(0.1, 0.1, 0.1, 1);
        textBatch.begin();
        auto offset = textBatch.submitText(textPos.x, textPos.y, scale, text);
        textBatch.flush();

        batch.begin();
        batch.fill({112, 112, 112, 255});
        if (int(window.getTime() * 2) % 2) batch.submitRect(textPos.x + offset, textPos.y, 2 * scale, 28 * 4 * scale);
        batch.flush();

        window.update();
    }
}
