#include <Coel.hpp>

int main() {
    Coel::Window window{"Shader Varying Variables Example"};
    Coel::create(window);

    Coel::Vao vao;
    Coel::create(vao);
    float vertex_data[]{
        -0.5, -0.5, 0, 0, 0, 1, // Black
        -0.5, 0.5, 1, 0, 0, 1,  // Red
        0.5, -0.5, 0, 1, 0, 1,  // Green
        0.5, 0.5, 0, 0, 1, 1    // Blue
    };
    Coel::Vbo vbo{{{Coel::F32, 2}, {Coel::F32, 4}}};
    Coel::create(vbo, vertex_data, sizeof(vertex_data));
    Coel::link(vao, vbo);
    unsigned int index_data[]{0, 1, 2, 1, 3, 2};
    Coel::Ibo ibo;
    Coel::create(ibo, index_data, sizeof(index_data));

    // --------------------------------------------------------------
    // We add a varying variable to both of our shaders. We can
    // set this per vertex by creating another attribute in our
    // Vbo above, and grabbing it from `location = 1`.

    // In `void main()`, we must assign the value of our attribute
    // to the varying variable, and then it will be the correct
    // value in our fragment shader. This will result in the pixels
    // in between being linearly interpolated between each vertex
    // color.
    // --------------------------------------------------------------
    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec2 a_pos;
    layout (location = 1) in vec4 a_col;
    out vec4 v_color;
    void main() {
        v_color = a_col;
        gl_Position = vec4(a_pos, 0, 1);
    })";

    const char *const fragSrc = R"(
    #version 450 core
    in vec4 v_color;
    out vec4 frag_color;
    void main() {
        frag_color = v_color;
    })";

    Coel::Shader shader;
    Coel::create(shader, vertSrc, fragSrc);

    while (window.isOpen) {
        Coel::Renderer::clearColor();
        Coel::bind(shader);
        Coel::Renderer::drawIndexed(vao, 6);
        Coel::update(window);
    }

    Coel::destroy(shader);
    Coel::destroy(ibo);
    Coel::destroy(vbo);
    Coel::destroy(vao);
    Coel::destroy(window);
}
