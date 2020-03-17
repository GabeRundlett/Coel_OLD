#include <Coel.hpp>

int main() {
    Coel::Window window(800, 600, "Simplest Window Example");

    float vertex_data[]{-0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5};
    Coel::Vbo vbo(vertex_data, sizeof(vertex_data), {{Coel::Element::F32, 2}});

    Coel::Vao vao;
    vao.add(vbo);

    unsigned int index_data[]{0, 1, 2, 1, 3, 2};
    Coel::Ibo ibo(index_data, sizeof(index_data));

    // --------------------------------------------------------------
    // To keep things simple, I'll write the shaders inline in a
    // raw string literal instead of loading them from a file.

    // In the vertex shader, we'll take in our position vec2 data,
    // (which is put into location 0 by our vao because it is the
    // first element in the initializer list of our vbo) and define
    // gl_Position with it. gl_Position is a vec4, so we must provide
    // the `.z` and `.w` components.
    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec2 a_pos;
    void main() {
        gl_Position = vec4(a_pos, 0, 1);
    }
    )";

    // Then in our fragment shader, we'll just define an output color
    // `frag_color`, which is a vec4, which is the default output of
    // the framebuffer created by our window manager.
    const char *const fragSrc = R"(
    #version 450 core
    out vec4 frag_color;
    void main() {
        frag_color = vec4(1, 0, 1, 1);
    }
    )";

    // Then we can just use these strings to compile a shader program
    Coel::Shader shader(vertSrc, fragSrc);
    // --------------------------------------------------------------

    while (window.isOpen()) {
        Coel::Renderer::clearColor();

        // --------------------------------------------------------------
        // Now all we do in the application loop is bind the shader
        // before we submit our draw call. This is not entirely necessary
        // since we only have 1 shader, but would be if we were using
        // multiple, so I'll show it now.
        shader.bind();
        // --------------------------------------------------------------
        vao.drawIndexed(6);

        window.update();
    }
}
