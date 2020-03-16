#include <Coel.hpp>

int main() {
    Coel::Window window(800, 600, "Simple Static Vertex Buffer Example");

    float vdata[]{
        -0.5, -0.5, // bottom left
        0.5,  -0.5, // bottom right
        0.0,  0.5,  // top middle
    };

    Coel::Vbo vbo(vdata, sizeof(vdata),
                  {
                      {Coel::Element::F32, 2},
                  });

    Coel::Vao vao;
    vao.add(vbo);

    while (window.isOpen()) {
        Coel::Renderer::clear();
        vao.draw(3);
        window.update();
    }
}
