#include <Coel.hpp>

int main() {
    Coel::Window window(800, 600, "Simple Static Index Buffer Example");

    float vdata[]{
        -0.5, -0.5, // bottom left 
        -0.5,  0.5, // top left
         0.5, -0.5, // bottom right
         0.5,  0.5, // top right
    };

    Coel::Vbo vbo(vdata, sizeof(vdata), {
        {Coel::Element::F32, 2},
    });
    
    Coel::Vao vao;
    vao.add(vbo);

    unsigned short idata[]{
        0, 1, 2, // Bottom left tri
        1, 3, 2, // Top right tri
    };

    Coel::Ibo ibo(idata, sizeof(idata));

    while (window.isOpen()) {
        Coel::Renderer::Command::clear();

        ibo.bind();
        vao.drawIndexed(6);
        
        window.update();
    }
}
