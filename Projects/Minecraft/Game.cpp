#include "Game.hpp"

#include "Chunk.hpp"
#include "Player.hpp"

namespace Minecraft {
    static Player p;
    static Chunk chunk;

    Game::Game() : window(1200, 800, "Window") {
        window.onResize = [](Coel::Window &w) {
            Coel::Renderer::Command::resizeViewport(0, 0, w.size.x, w.size.y);
            p.windowSizeUpdate(w.size);
            w.cursorTo(p.mouseCenter);
        };
        window.onKey = [](Coel::Window &w) {
            p.keyUpdate(w.key); //
        };
        window.onMouseMove = [](Coel::Window &w) {
            p.mouseUpdate(w.mouse);
            w.cursorTo(p.mouseCenter);
        };
        window.onResize(window);

        onInit();
    }

    void Game::onInit() {
        p.setPos({0, 0, 0}); //
        p.setRot({0, 0, 0}); //
        chunk.update();
    }

    bool Game::shouldRun() {
        return window.isOpen(); //
    }

    void Game::updateTime() {
        prevTime = time, time = window.getTime();
        elapsed = time - prevTime;
        Coel::Renderer::Command::clear();
    }

    void Game::update() {
        updateTime();
        p.update((float)elapsed);
        renderer.updateCamera(p.cam);
        onUpdate();
        renderer.flush();
        window.update();
    }

    void Game::onUpdate() {
    }
} // namespace Minecraft
    