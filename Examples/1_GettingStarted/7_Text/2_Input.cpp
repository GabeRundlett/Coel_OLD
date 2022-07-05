#include <Coel.hpp>
#include <iostream>

int main() {
    Coel::Window window({100, 100}, "Text Input Example");
    Coel::create(window);
    std::string text = "";

    window.onKey = [&text](const Coel::Window &w) {
        switch (w.key.action) {
        case Coel::Action::Press:
        case Coel::Action::Repeat:
            if (w.key.code == Coel::Key::Backspace) {
                std::size_t pos = text.size() - 1;
                if (w.key.mods & Coel::Mod::Control)
                    pos = text.rfind(' ');
                if (pos == std::string::npos)
                    pos = 0;
                text = text.substr(0, pos);
                // std::cout << "\033[K\033[1000D" << text << std::flush;
                // for (unsigned int i = 0; i < 100; ++i)
                // std::cout << ' ';
                // std::cout << std::flush;
            }
            break;
        default: break;
        }
    };

    window.onChar = [&text](Coel::Window &w) {
        switch (w.key.action) {
        case Coel::Action::Press:
        case Coel::Action::Repeat:
            text.push_back(w.key.character);
            // std::cout << "\033[K\033[1000D" << text << std::flush;
            // for (unsigned int i = 0; i < 100; ++i)
            // std::cout << ' ';
            // std::cout << std::flush;
            break;
        default: break;
        }
    };

    while (window.isOpen) {
        Coel::update(window);
    }

    Coel::destroy(window);
}
