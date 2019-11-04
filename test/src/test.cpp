#include <coel.hpp>
#include <iostream>
#include <string>

namespace test {
    static constexpr math::Color            //
        background_col = {18, 18, 18, 255}, //
        col1 = {30, 30, 30, 255},           //
        col2 = {150, 60, 60, 255},          //
        col3 = {60, 60, 150, 255};          //

    static std::string text_box_string = "Hello, World!";

    static inline void draw_x(const math::Vec2 &pos, const float size, const float radius) {
        using namespace coel::renderer::batch2d;
        fill_color(col2);
        fill_line({{pos.x - size + radius, pos.y - size + radius}, {pos.x + size - radius, pos.y + size - radius}, {radius}});
        fill_line({{pos.x + size - radius, pos.y - size + radius}, {pos.x - size + radius, pos.y + size - radius}, {radius}});
    }

    static inline void draw_o(const math::Vec2 &pos, const float size, const float radius) {
        using namespace coel::renderer::batch2d;
        fill_color(col3);
        fill_ellipse({{pos.x - size, pos.y - size}, {size * 2, size * 2}});
        fill_color(background_col);
        fill_ellipse({{pos.x - size + radius * 2, pos.y - size + radius * 2}, {size * 2 - radius * 4, size * 2 - radius * 4}});
    } // namespace test

    struct Window : coel::Window {
        Window(const unsigned int w, const unsigned int h, const char *const t) : coel::Window(w, h, t) {
            coel::renderer::batch2d::init(w, h);
            coel::renderer::clear_color(background_col);
            coel::renderer::clear();
        }

        void key_press(const coel::KeyPress &e) override {
            char text[2] = "1";
            switch (e.key) {
            case coel::Backspace:
                //
                break;
            case coel::Tab:
                //
                break;
            case coel::Enter:
            case coel::NumpadEnter:
                //
                break;
            default:
                //
                switch (e.mods) {
                case coel::Shift: break;
                case coel::Alt: break;
                case coel::Control: break;
                default: break;
                }
                break;
            }
            text[0] = e.key;
            // text_box_string.append(text);
            std::cout << char(e.key);
        }

        void on_update() override {
            using namespace coel::renderer::batch2d;
            fill_color({30, 30, 30, 255});
            fill_rect({{90, 110}, {400, -34}});
            fill_color({150, 150, 150, 255});
            fill_text({text_box_string.c_str(), {100, 100}});
        }
    };
} // namespace test

int main() {
    test::Window window(1600, 800, "title");
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
