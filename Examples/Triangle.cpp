#include <Coel.hpp>

int main() {
    auto window = Coel::createWindow(800, 600, "Simple Triangle Example");

    struct Vertex {
        Math::Vec2 pos;
    };
    Vertex vertices[] = {
        {-0.5, -0.5},
        {0.5, -0.5},
        {0, 0.5},
    };
    unsigned short indices[] = {0, 1, 2};

    auto vbo = Coel::createVbo(vertices, sizeof(vertices), {{Coel::Type::f32, 2}});
    auto ibo = Coel::createIbo(indices, sizeof(indices));
    auto vao = Coel::createVao();
    vao->addVbo(vbo);

    while (!window->shouldClose()) {
        Coel::Renderer::clear();

        vao->draw(ibo);
        window->update();
    }
}
