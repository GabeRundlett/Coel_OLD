#include <Coel.hpp>

int main() {
    auto window = Coel::createWindow(800, 600, "Window Test");

    while (!window->shouldClose()) {
        window->update();
    }
}
