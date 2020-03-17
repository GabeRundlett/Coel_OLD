#include <Coel.hpp>

int main() {
    Coel::Window window(800, 600, "Simplest Triangle (With VBO) Example");

    // --------------------------------------------------------------
    // First, we must create a vertex array. This allows us to
    // control all of our vertex buffers with one object.
    Coel::Vao triangle_vao;

    // Then we create the data for each vertex. This is laid out
    // in a specific order that we'll need to specify when we create
    // the vertex buffer.
    float vertex_data[]{
        -0.5, -0.5, // bottom left
        -0.5, 0.5,  // top left
        0.5,  -0.5  // bottom right
    };
    // Now we can create our triangle vertex buffer. We pass a pointer
    // to the data, the size in bytes of the buffer, and then an initializer
    // list specifying the aforementioned layout. This takes a list of
    // "Elements", which are constructed as such: {Enum(type), count}
    Coel::Vbo triangle_vbo(vertex_data, sizeof(vertex_data), {{Coel::Element::F32, 2}});

    // Now that our buffer is created, we can attach it to the vertex
    // array. This is done via a member function of Vao. We must keep the
    // triangle_vbo object alive (and thus can't construct it as an argument)
    // because its destructor does the necessary cleanup, deleting the vbo.
    triangle_vao.add(triangle_vbo);
    // --------------------------------------------------------------

    while (window.isOpen()) {
        Coel::Renderer::clearColor();

        // --------------------------------------------------------------
        // Once all the initialization is done, all that is necessary
        // in our application loop is to call the draw member function
        // of the triangle_vao with the specified vertex count we want
        // to draw from the buffer. If the buffer was larger and we
        // specified a number smaller than the number of vertices in
        // the buffer, it would render the maximum number of triangles
        // constructable from the specified count, which is always a
        // multiple of 3.
        triangle_vao.draw(3);
        // --------------------------------------------------------------

        window.update();
    }
}
