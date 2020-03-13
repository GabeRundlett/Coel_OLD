#include <Coel.hpp>
#include "Shaders.hpp"

int main() {
    Coel::Window window(800, 600, "Simple Static Vertex Buffer Example");
    Coel::Shader shader(vertSrc, fragSrc);
    Coel::Texture texture("C:/Assets/Images/face.png");

    float vdata[]{
        -1, -1, 0, 0, //
         1, -1, 1, 0, //
        -1,  1, 0, 1, //

        -1,  1, 0, 1, //
         1, -1, 1, 0, //
         1,  1, 1, 1, //
    };

    Coel::Vbo vbo(vdata, sizeof(vdata), {
        {Coel::Element::F32, 2},
        {Coel::Element::F32, 2},
    });

    Coel::Vao vao;
    vao.add(vbo);

    while (window.isOpen()) {
        Coel::Renderer::clear();

        shader.bind();
        texture.bind(0);
        vao.draw(6);
        
        window.update();
    }
}
