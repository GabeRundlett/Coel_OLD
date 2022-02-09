#include "0_Resources/Player.hpp"
#include "0_Resources/Shaders.hpp"

int main() {
    Player player;

    Coel::Window window("Simple Plane Generation");
    window.onMouseMove = [&player](Coel::Window &w) {
        player.mouseMoveUpdate(w.mouse); //
        Coel::cursorTo(w, player.mouseCenter);
    };
    window.onMouseButton = [&player](Coel::Window &w) {
        player.mouseButtonUpdate(w.mouse); //
    };
    window.onMouseScroll = [&player](Coel::Window &w) {
        player.mouseScrollUpdate(w.mouse); //
    };
    window.onKey = [&player](Coel::Window &w) {
        player.keyUpdate(w.key); //
    };
    window.onResize = [&player](Coel::Window &w) {
        player.windowSizeUpdate(w.size); //
    };

    Coel::create(window);
    Coel::cursorMode(window, Coel::Disabled);

    Coel::Renderer::Batch3d renderer;
    Coel::Renderer::init(renderer);

    Coel::Shader shader;
    Coel::create(shader, vertSrc, fragSrc);

    auto u_proj = Coel::findMat4(shader, "u_proj");
    auto u_view = Coel::findMat4(shader, "u_view");

    while (window.isOpen) {
        Coel::Renderer::clear();

        renderer.begin();
        for (float z = 0; z < 10; ++z) {
            for (float x = 0; x < 10; ++x) {
                auto s = sin((x + z) / 2) / 4;
                renderer.submitQuad({
                    {{x + 0, s, z + 0}, {0, 0}},
                    {{x + 0, s, z + 1}, {0, 1}},
                    {{x + 1, s, z + 0}, {1, 0}},
                    {{x + 1, s, z + 1}, {1, 1}},
                });
            }
        }
        Coel::send(u_proj, &player.cam.projMat);
        Coel::send(u_view, &player.cam.viewMat);
        Coel::bind(shader);
        renderer.flush();

        Coel::update(window);
        player.update(static_cast<float>(window.elapsed));
    }

    Coel::destroy(shader);
    Coel::Renderer::deinit(renderer);
    Coel::destroy(window);
}
