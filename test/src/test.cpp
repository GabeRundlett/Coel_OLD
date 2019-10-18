#include <coel.hpp>
#include <iostream>
#include <math.hpp>

namespace test {
    struct Window : coel::Window {
        math::dVec2 mouse = {0, 0};

        Window(const unsigned int w, const unsigned int h, const char *const t) : coel::Window(w, h, t) {
            coel::renderer::batch2d::init(w, h);
        }

        void mouse_move(const coel::MouseMove &e) override { //
            mouse = {e.x, e.y};
        }

        void on_update() override {
            coel::renderer::clear(0.5f);
            coel::renderer::batch2d::submit_rect(mouse.x, mouse.y, 100, 100);
            coel::renderer::batch2d::flush();
        }
    };
} // namespace test

int main() {
    test::Window window(1600, 800, "title");
    while (!window.should_close()) {
        window.update();
    }
}

// hi honey i love you !
