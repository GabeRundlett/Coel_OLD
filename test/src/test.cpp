#include <coel.hpp>
#include <iostream>

namespace test {
    static constexpr math::Color col = {255, 0, 255, 255};
    static float total_frame_time = 0.f;
    static unsigned int fpt = 0;

    struct Window : coel::Window {
        math::dVec2 mouse = {0, 0};

        Window(const unsigned int w, const unsigned int h, const char *const t) : coel::Window(w, h, t) {
            coel::renderer::batch2d::init(w, h);
            coel::renderer::batch2d::fill_color(0xff00ffff);
            coel::renderer::clear_color(0x0f0f0fff);
        }

        void mouse_move(const coel::MouseMove &e) override { //
            mouse = {e.x, e.y};
        }

        void on_update() override {
            const float time = get_time(), elapsed = time - total_frame_time;

            if (elapsed > 1.f) {
                std::cout << fpt << '\n';
                total_frame_time += 1.f, fpt = 0;
            }

            for (unsigned int y = 0; y < 100; ++y)
                for (unsigned int x = 0; x < 100; ++x) {
                    coel::renderer::batch2d::fill_color({x % 256, y % 256, 0, 255});
                    coel::renderer::batch2d::fill_rect(5.f * x, 5.f * y, 4, 4);
                }

            coel::renderer::batch2d::flush();
            ++fpt;
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
