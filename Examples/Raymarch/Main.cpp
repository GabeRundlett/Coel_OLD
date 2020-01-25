#include <Coel.hpp>

#include "Frag.hpp"
#include "Vert.hpp"

Math::Vec2i size{800, 600};

void windowResize(const Coel::WindowResize &e) { size = e.size; }

int main() {
    auto window = Coel::createWindow(size.x, size.y, "Raymarching Example");

    struct Vertex {
        Math::Vec2 pos, tex;
    };
    Vertex vertices[] = {
        {-1, -1, 0, 0},
        {1, -1, 1, 0},
        {-1, 1, 0, 1},
        {1, 1, 1, 1},
    };
    unsigned short indices[] = {0, 1, 2, 1, 3, 2};

    auto vbo = Coel::createVbo(vertices, sizeof(vertices), {{Coel::Type::f32, 2}, {Coel::Type::f32, 2}});
    auto ibo = Coel::createIbo(indices, sizeof(indices));
    auto vao = Coel::createVao();
    vao->addVbo(vbo);
    auto shader = Coel::createShader(sVertSrc, sFragSrc);

    while (!window->shouldClose()) {
        Coel::Renderer::clear();

        vao->draw(ibo);
        window->update();
    }
}
