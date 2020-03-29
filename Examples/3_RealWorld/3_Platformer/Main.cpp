#include <Coel.hpp>

Coel::Window window(1280, 900, "Platformer");

namespace Renderer {
    struct Vertex {
        glm::vec2 pos, tex;
    };

    const unsigned int vCount = 10000, iCount = 10000;

    Coel::Vao vao;
    Coel::Vbo vbo(nullptr, sizeof(Vertex) * vCount, {{Coel::Element::F32, 2}, {Coel::Element::F32, 2}});
    Coel::Ibo ibo(nullptr, sizeof(unsigned int) * iCount);

    Vertex *vertices;
    unsigned int *indices;

    void init() {
        vao.add(vbo);
        //
    }

    void begin() {
        vao.bind();
        vbo.open(&vertices);
        ibo.open(&indices);
    }

    void flush() {
        //
    }
} // namespace Renderer

int main() {
    Renderer::init();

    while (window.isOpen()) {
        window.update();
    }
}
