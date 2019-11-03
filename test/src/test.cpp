#include <coel.hpp>

namespace test {
    static constexpr math::Color background_col = {18, 18, 18, 255}, col1 = {180, 180, 180, 100}, col2 = {255, 180, 100, 100};
    static float total_frame_time = 0.f;
    static unsigned int fpt = 0;

    struct Window : coel::Window {
        math::Vec2 mouse = {0, 0};

        Window(const unsigned int w, const unsigned int h, const char *const t) : coel::Window(w, h, t) {
            coel::renderer::batch2d::init(w, h);
            coel::renderer::clear_color(background_col);
            coel::renderer::clear();
        }

        void mouse_move(const coel::MouseMove &e) override { //
            mouse = {float(e.x), float(e.y)};
        }

        void on_update() override {
            // coel::renderer::batch2d::fill_rect({{mouse.x, mouse.y}, {400, 400}});
            coel::renderer::batch2d::fill_color(col2);
            coel::renderer::batch2d::fill_line({{mouse.x, mouse.y}, {400, 400}, {10}});
            coel::renderer::batch2d::fill_color(col1);
            coel::renderer::batch2d::fill_ellipse({{mouse.x - 5, mouse.y - 5}, {10, 10}});
            coel::renderer::batch2d::fill_ellipse({{400 - 5, 400 - 5}, {10, 10}});
        }
    };
} // namespace test

int main() {
    test::Window window(1600, 900, "title");
    while (!window.should_close()) {
        window.update();
        coel::renderer::batch2d::flush();
    }
}

// hi honey i love you !

/*
VSCode
            // bottom bar
            coel::renderer::batch2d::fill_color({0, 122, 204, 255});
            coel::renderer::batch2d::fill_rect(0, height - 21, width, 21);
            // side bar
            coel::renderer::batch2d::fill_color({51, 51, 51, 255});
            coel::renderer::batch2d::fill_rect(0, 0, 43, height - 21);
            // file explorer
            coel::renderer::batch2d::fill_color({37, 37, 38, 255});
            coel::renderer::batch2d::fill_rect(43, 0, 348, height - 21);
            // terminal
            coel::renderer::batch2d::fill_color({55, 55, 55, 255});
            coel::renderer::batch2d::fill_rect(348 + 43, height - 370 - 21, width - 348 - 43, 1);
            // button
            coel::renderer::batch2d::fill_rect(width - 350, height - 370 - 21 + 7, 160, 21, 4);

Material

    static inline void rect_and_shadow(const float x, const float y, const float w, const float h, const float r = 0.f) {
        coel::renderer::batch2d::fill_color(button_shadow_col);
        coel::renderer::batch2d::shadow(x, y, w, h);
        coel::renderer::batch2d::fill_color(button_col);
        coel::renderer::batch2d::fill_rect(x, y, w, h);
    }


            rect_and_shadow(000, 000, 328, 50);

            rect_and_shadow(000 + 16, 100, 140, 100, 5);
            rect_and_shadow(156 + 16, 100, 140, 180, 5);
            rect_and_shadow(000 + 16, 216, 140, 160, 5);
            rect_and_shadow(156 + 16, 296, 140, 110, 5);

Button


    struct Button {
        math::Vec3 pos;
        math::Vec2 size;
        bool hovered = false, pressed = false, animate = false;
        float animate_speed = 0.1f;

        Button(float x, float y, float w, float h) : pos{x, y, 0}, size{w, h} {}
        Button(const math::Vec2 &pos, const math::Vec2 &size) : pos{pos.x, pos.y, 0}, size{size} {}

        inline void hover(const math::Vec2 &mouse_pos) {
            const bool prev_hovered = hovered;
            hovered = mouse_pos.x > pos.x && mouse_pos.x < pos.x + size.x && //
                      mouse_pos.y > pos.y && mouse_pos.y < pos.y + size.y;
            if (prev_hovered != hovered) {
                if (hovered)
                    animate_speed = 0.01f;
                else
                    animate_speed = -0.01f;
                animate = true;
            }
        }

        inline void press() {
            pressed = hovered;
            //
        }

        inline void update() {
            if (animate) {
                pos.z += animate_speed;
                if (pos.z > 0.3f) {
                    pos.z = 0.3f;
                    animate = false;
                } else if (pos.z < 0.f) {
                    pos.z = 0.f;
                    animate = false;
                }
            }
        }

        inline void draw() { coel::renderer::batch2d::fill_rect({pos, size}); }
    };

*/
