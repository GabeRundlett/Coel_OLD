#include <Coel.hpp>

int main() {
    // --------------------------------------------------------------
    // We can forward declare, and even default construct
    // the window object, which will use defaults for the
    // parameters necessary in the initialization.
    Coel::Window window;
    // First, we'll initialize the window. This will
    // initialize the windowing library and actually also
    // creates the Rendering Context, so we will always
    // have to do this first.
    Coel::create(window);

    // Then, we create the main application loop. This
    // loops until the window is closed, as according to
    // to the loop condition checking if the window is
    // open or not.
    while (window.isOpen) {

        // Every frame, we update the window. This swaps the
        // buffers, as well as polls system events such as
        // key input. Since the window close action is handled
        // as an event, we must update the window in order for
        // it to close.
        Coel::update(window);
    }

    // Once we're done with the window, we'll deinitialize all of
    // its resources. This will terminate the window manager, so
    // when creating a new window after, it will reinitialize.
    Coel::destroy(window);
    // --------------------------------------------------------------
}
