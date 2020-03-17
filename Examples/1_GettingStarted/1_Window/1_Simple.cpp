#include <Coel.hpp>

int main() {
    // --------------------------------------------------------------
    // First, we'll create the Window object. This will
    // initialize the windowing library and actually also
    // creates the Rendering Context, so we will always
    // have to do this first.
    Coel::Window window(800, 600, "Simplest Window Example");

    // Then, we create the main application loop. This
    // loops until the window is closed, as according to
    // to the loop condition checking if the window is
    // open or not.
    while (window.isOpen()) {

        // Every frame, we update the window. This swaps the
        // buffers, as well as polls system events such as
        // key input. Since the window close action is handled
        // as an event, we must update the window in order for
        // it to close.
        window.update();
    }
    // --------------------------------------------------------------
}
