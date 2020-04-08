#include <Coel.hpp>

int main() {
    Coel::Window window{"Indexed Triangle Example"};
    Coel::create(window);

    float vertex_data[]{
        -0.5, -0.5, // 1----3 <-- New
        -0.5, 0.5,  // |`.  |   Vertex!
        0.5,  -0.5, // |  `.|
        0.5,  0.5   // 0----2
    };
    Coel::Vbo vbo{{{Coel::F32, 2}}};
    Coel::create(vbo, vertex_data, sizeof(vertex_data));
    Coel::Vao vao;
    Coel::create(vao);
    Coel::link(vao, vbo);

    // --------------------------------------------------------------
    // One thing to immediately note is that I added a fourth
    // vertex in the vbo. This is to demonstrate the value of
    // indexing. I also created the Vao after the Vbo. This is
    // unnecessary and I only did it to show that their order of
    // creation is independent.

    // First, we'll create our indices. These must be 4 byte
    // unsigned integers and so we'll use `unsigned int`.
    // These indices tell the GPU which vertices to use for
    // each triangle. In the diagram above, you can see we
    // made two triangles, one out of the vertices '0, 1, 2'
    unsigned int index_data[]{0, 1, 2, 1, 3, 2};
    // Now we'll create our index buffer. We just do so
    // in the same way as the vbo, but we don't need any
    // layout, since it's known that they are unsigned ints.
    Coel::Ibo ibo;
    Coel::create(ibo, index_data, sizeof(index_data));
    // --------------------------------------------------------------

    while (window.isOpen) {
        Coel::Renderer::clearColor();

        // --------------------------------------------------------------
        // Now that our Index Buffer has been created, we can call
        // our draw function. Note that we call drawIndexed over
        // the previous `vao.draw(3);`. We specify 6 since we're
        // rendering 6 indices, which will connect the vertices of
        // two triangles.
        Coel::Renderer::drawIndexed(vao, 6);
        // --------------------------------------------------------------

        Coel::update(window);
    }
}
