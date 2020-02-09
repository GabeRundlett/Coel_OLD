#include <Coel.hpp>

int main() {
	Coel::Window window(800, 600, "Window");

	while (window.isOpen()) {
		window.update();
	}
}
