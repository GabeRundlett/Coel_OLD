#include "0_Resources/1_Renderer/Renderer.hpp"
#include "0_Resources/1_Renderer/Shaders.hpp"
#include "0_Resources/2_Player/Player.hpp"

int main() {
    Coel::Window window("Minecraft Example - 1: Renderer");
    Coel::create(window);

    Coel::Renderer::init(batch, MAX_VSIZE, MAX_ISIZE);

    Coel::Shader shader;
    Coel::create(shader, vertSrc, fragSrc);
    auto u_proj = Coel::findMat4(shader, "u_proj");
    auto u_view = Coel::findMat4(shader, "u_view");
    Coel::Texture texture;
    Coel::create(texture, "Assets/cobblestone.png");
    Coel::setMagFilter(texture, Coel::Nearest);
    Player player;
    player.init();

    window.onResize = [&player, &shader, &u_proj, &u_view](Coel::Window &w) {
        player.updateCamera(shader, w.size, u_proj, u_view);
    };
    Coel::resize(window);

    window.onKey = [&player](Coel::Window &w) { player.onKey(w.key); };

    window.onMouseMove = [&player](Coel::Window &w) {
        player.onMouseMove(w.mouse);
        Coel::cursorTo(w, player.mouseCenter);
    };

    Coel::cursorMode(window, Coel::CursorMode::Disabled);

    Coel::Renderer::enableCulling(true);
    Coel::Renderer::enableDepthTest(true);
    Coel::Renderer::setClearColor(0.72f, 0.76f, 1.0f, 1.0f);
    auto time = Coel::getTime(), prevTime = time, elapsed = time;

    while (window.isOpen) {
        prevTime = time, time = Coel::getTime();
        elapsed = time - prevTime;

        Coel::Renderer::clear();
        begin();

        player.update(static_cast<float>(elapsed));
        player.updateCamera(shader, u_proj, u_view);

        for (float x = 0; x < 4; ++x)
            for (float y = 0; y < 4; ++y)
                for (float z = 0; z < 4; ++z)
                    submitCube({x - 1.5f, y - 1.5f, z - 5.0f});

        Coel::bind(shader);
        // Coel::send(u_tex, 0);
        flush();
        Coel::update(window);
    }

    Coel::Renderer::deinit(batch);

    Coel::destroy(window);
}
