#include <coel.hpp>

class MyApp final : public coel::App {
    coel::Ref<coel::Window> m_window;

  public:
    MyApp() {}
};

coel::Scope<coel::App> coel::create_app() { return coel::create_scope<MyApp>(); }

/*
int main() {
    auto window = coel::create_window(800, 800, "Title");
    window->on_key_press([](const coel::KeyPress &e) {
        std::cout << "Key Pressed!\n"; //
    });

    float vertices[] = {-0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5};
    unsigned short indices[] = {0, 1, 2, 1, 3, 2};
    auto vbo = coel::create_vbo(vertices, sizeof(vertices), {{coel::Type::f32, 2}});
    auto ibo = coel::create_ibo(indices, sizeof(indices));
    auto vao = coel::create_vao();
    vao->add_vbo(vbo);

    double prev_time = window->get_time();
    size_t fps = 0;

    while (window->is_open()) {
        vao->draw(ibo);
        window->update();
    }
}
*/

// hi honey i love you !
