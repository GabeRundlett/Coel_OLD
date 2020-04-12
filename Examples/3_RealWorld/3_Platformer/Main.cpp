#include <Coel.hpp>

int main() {
    Coel::Window window("Platformer Example");
    Coel::create(window);

    {
        Coel::Shader shader(vertSrc, fragSrc);

        while (window.isOpen) {
            Coel::Renderer::clearColor();
            shader.bind();
            Coel::update(window);
        }
    }

    Coel::destroy(window);
}
