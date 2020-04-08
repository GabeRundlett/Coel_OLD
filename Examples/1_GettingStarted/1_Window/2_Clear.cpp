#include <Coel.hpp>

int main() {
    Coel::Window window{"Clearing Window Example"};
    Coel::create(window);

    while (window.isOpen) {
        // --------------------------------------------------------------
        // One can either clear the color buffer,
        Coel::Renderer::clearColor();
        // The depth buffer (which won't solve the strange
        // clearing issue, but will help in the future),
        //  - Coel::Renderer::clearDepth();
        // or even simpler, clear both the color and depth
        // buffer by calling:
        //  - Coel::Renderer::clear();
        // --------------------------------------------------------------
        Coel::update(window);
    }

    Coel::destroy(window);
}
