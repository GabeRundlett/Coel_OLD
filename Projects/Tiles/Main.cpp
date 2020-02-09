#include <Coel.hpp>

#include "Shaders.hpp"

Coel::Window window(1400, 900, "Tiles");

Coel::Renderer::Batch2d renderer;
Coel::Shader shader(vertSrc, fragSrc);

Math::Mat4 viewMat;

int main() {
    window.onResize = [](Coel::Window &w) {
        viewMat = Math::Mat4::ortho(0, w.size.x, w.size.y, 0, -1, 1);
        shader.sendMat4("viewMat", &viewMat);
    };
    window.onResize(window);

    Coel::Renderer::Command::setClearColor(0.1f, 0.1f, 0.1f, 1.f);

    while (window.isOpen()) {
        Coel::Renderer::Command::clear();

        constexpr unsigned int tileCountX = 50, tileCountY = 30, tileWidth = 25, tileHeight = 25;
        for (unsigned int yi = 0; yi < tileCountY; ++yi) {
            for (unsigned int xi = 0; xi < tileCountX; ++xi) {
                renderer.submitRect(xi * tileWidth, yi * tileHeight, tileWidth - 1, tileHeight - 1);
            }
        }

        renderer.flush();
        window.update();
    }
}
