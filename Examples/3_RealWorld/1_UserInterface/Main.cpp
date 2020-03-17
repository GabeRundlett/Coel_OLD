#include <Coel.hpp>

int main() {
    Coel::Window window(1400, 800, "User Interface Example");

    while (window.isOpen()) {

        window.update();
    }
}
