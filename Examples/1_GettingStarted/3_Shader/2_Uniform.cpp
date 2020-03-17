#include <Coel.hpp>

int main() {
    Coel::Window window(800, 600, "Shader Uniforms Example");

    float vertex_data[]{-0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5};
    Coel::Vbo vbo(vertex_data, sizeof(vertex_data), {{Coel::Element::F32, 2}});

    Coel::Vao vao;
    vao.add(vbo);

    unsigned int index_data[]{0, 1, 2, 1, 3, 2};
    Coel::Ibo ibo(index_data, sizeof(index_data));

    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec2 a_pos;
    void main() {
        gl_Position = vec4(a_pos, 0, 1);
    }
    )";

    // --------------------------------------------------------------
    // We add a uniform variable to our shader. We'll remember
    // the name of this variable because we'll use it to find
    // the location of it via our c++ code.
    // --------------------------------------------------------------
    const char *const fragSrc = R"(
    #version 450 core

    layout (location = 1) uniform vec4 u_color = vec4(0, 1, 0, 1);
    
    out vec4 frag_color;
    void main() {
        frag_color = u_color;
    }
    )";

    Coel::Shader shader(vertSrc, fragSrc);

    // --------------------------------------------------------------
    // We also need to find the location of our uniform from our
    // shader. Do this here, and store it in a Coel::Shader::Uniform.
    auto u_color = shader.findFloat4("u_color");
    // --------------------------------------------------------------

    while (window.isOpen()) {
        Coel::Renderer::clearColor();

        shader.bind();

        // --------------------------------------------------------------
        // After our shader is bound, we can send a value to the
        // uniform location we found previously.

        // In this, I'll also grab the time and modulo it to modify
        // the color uniform every frame.
        int value = (int)(window.getTime() * 100) % 100;
        // Then we'll create our color variable:
        glm::vec4 color{(float)value / 100, 0, 0, 1};
        // and finally send it to the shader via our uniform.
        shader.send(Coel::Shader::Uniform<glm::vec4>{1}, &color);
        // --------------------------------------------------------------

        vao.drawIndexed(6);

        window.update();
    }
}
