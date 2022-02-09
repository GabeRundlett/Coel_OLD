#include <Coel.hpp>
#include <iostream>

int main() {
    Coel::Window window{"Window Events Example"};

    // --------------------------------------------------------------
    // In order to process events from the application, all
    // we need to do is define a callback function. This will
    // be called when the window is updated.

    // All of the events that are handled by the engine, when
    // triggered, call a callback std::function owned by the
    // window object above. Just before the callback is called,
    // the corresponding data of the event is updated inside
    // the window.

    // There are multiple different event callbacks in the
    // window object:

    //  - onResize
    //  - onFboResize

    //  - onMouseButton
    //  - onMouseMove
    //  - onMouseScroll

    //  - onKey
    //  - onChar

    // Since we're using a std::function for each of the
    // callbacks, we can easily capture local variables into
    // our lambda expression.
    int keyEventCount = 0;

    window.onKey = [&keyEventCount](Coel::Window &w) {
        // Window::key has 4 fields:
        // - key.action:
        //    This will be one of [Coel::Action::Press, Release, Repeat]
        // - key.code:
        //    This is the id of the key being (pressed/released..etc)
        // - key.mods
        //    This is a bitmask of the id/s of all the mods being used,
        //    which are [Coel::Mods::Alt, CapsLock, Shift, Control, Windows]
        // - key.character [IMPORTANT]
        //    This field is not updated by the event that calls this callback
        //    and thus will not be updated accordingly. To correctly use this
        //    field, use the Window::onChar callback. This
        if (w.key.action == Coel::Action::Press)
            std::cout << "Pressed key  [" << w.key.code << " '" << static_cast<char>(w.key.character) << "']\n";
        if (w.key.action == Coel::Action::Repeat)
            std::cout << "Repeated key [" << w.key.code << " '" << static_cast<char>(w.key.character) << "']\n";
        if (w.key.action == Coel::Action::Release)
            std::cout << "Released key [" << w.key.code << " '" << static_cast<char>(w.key.character) << "']\n";
        std::cout << ++keyEventCount << '\n';
    };
    // --------------------------------------------------------------

    Coel::create(window);
    while (window.isOpen) {
        Coel::update(window);
    }
    Coel::destroy(window);
}
