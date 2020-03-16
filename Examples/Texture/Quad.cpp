#include <Coel.hpp>
#include "Shaders.hpp"

int main() {
    Coel::Window window(800, 600, "Simple Textured Quad Example");
    Coel::Renderer::Quad2d quad;
    Coel::Shader shader(vertSrc, fragSrc);
    Coel::Texture texture("C:/Dev/Coel/Assets/face.png");

    while (window.isOpen()) {
        Coel::Renderer::clearColor();

        shader.bind();
        texture.bind(0);
        quad.draw();

        window.update();
    }
}
