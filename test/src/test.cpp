#include <coel.hpp>

int main() {
    coel::Window window(1600, 900, "title");
    coel::gui::init();

    while (!window.should_close()) {
        coel::renderer::clear(0.5f);
        coel::gui::draw_rect(0, 0, 1, 1);
        coel::gui::flush();
        window.update();
    }
}

// hi honey i love you !
