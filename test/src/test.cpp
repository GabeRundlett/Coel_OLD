#include <coel.hpp>

math::Vec2 vertices[] = {-0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5};
unsigned short indices[] = {0, 1, 2, 1, 3, 2};
class MyApp final : public coel::App {
    coel::Ref<coel::Vbo> vbo = coel::create_vbo(vertices, sizeof(vertices), {{coel::Type::f32, 2}});
    coel::Ref<coel::Ibo> ibo = coel::create_ibo(indices, sizeof(indices));
    coel::Ref<coel::Vao> vao = coel::create_vao();

  public:
    MyApp() { vao->add_vbo(vbo); }
    void update() override {
        vao->draw(ibo); //
    }
};

coel::Scope<coel::App> coel::create_app() { return coel::create_scope<MyApp>(); }

// hi honey i love you !
