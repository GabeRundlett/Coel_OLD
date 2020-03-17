#include <Coel.hpp>

int main() {
    Coel::Window window(800, 600, "Indexed Triangle Example");

    float vertex_data[]{-0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5};
    Coel::Vbo vbo(vertex_data, sizeof(vertex_data), {{Coel::Element::F32, 2}});

    Coel::Vao vao;
    vao.add(vbo);

    // --------------------------------------------------------------
    // One thing to immediately note is that I added an extra
    // vertex in the vbo. This is to demonstrate the value of
    // indexing. I also created the Vao after the Vbo. This is
    // unecessary and I only did it to show that their order of
    // creation is independent.

    // First, we'll create our indices. These must be 4 byte
    // unsigned integers and so we'll use `unsigned int`.
    unsigned int index_data[]{0, 1, 2, 1, 3, 2};
    // Now we'll create our index buffer. We just do so
    // in the same way as the vbo, but we don't need any
    // layout, since it's known that they are unsigned ints.
    Coel::Ibo ibo(index_data, sizeof(index_data));
    // --------------------------------------------------------------

    while (window.isOpen()) {
        Coel::Renderer::clearColor();

        // --------------------------------------------------------------
        // Now that our Index Buffer has been created, we can call
        // our draw function. Note that we call drawIndexed over
        // the previous `vao.draw(3);`. We specify 6 since we're
        // rendering 6 indices, which will connect the vertices of
        // two triangles.
        vao.drawIndexed(6);
        // --------------------------------------------------------------

        window.update();
    }
}
