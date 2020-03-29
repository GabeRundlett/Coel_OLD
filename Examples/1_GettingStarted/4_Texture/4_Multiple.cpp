#include <Coel.hpp>

int main() {
    Coel::Window window(512, 512, "Multiple Textures Example");

    float vertex_data[]{-1, -1, 0, 0, -1, 1, 0, 1, 1, -1, 1, 0, 1, 1, 1, 1};
    Coel::Vbo vbo(vertex_data, sizeof(vertex_data), {{Coel::Element::F32, 2}, {Coel::Element::F32, 2}});

    Coel::Vao vao;
    vao.add(vbo);

    unsigned int index_data[]{0, 1, 2, 1, 3, 2};
    Coel::Ibo ibo(index_data, sizeof(index_data));

    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec2 a_pos;
    layout (location = 1) in vec2 a_tex;
    out vec2 v_tex;
    void main() {
        v_tex = a_tex;
        gl_Position = vec4(a_pos, 0, 1);
    }
    )";

    const char *const fragSrc = R"(
    #version 450 core
    in vec2 v_tex;
    out vec4 frag_color;
    uniform sampler2D u_tex1;
    uniform sampler2D u_tex2;
    void main() {
        frag_color = (texture(u_tex1, v_tex) + texture(u_tex2, v_tex)) / 2;
    }
    )";

    Coel::Shader shader(vertSrc, fragSrc);

    auto u_tex1 = shader.findInt("u_tex1");
    Coel::Texture texture1("Assets/UVGrid.png");
    auto u_tex2 = shader.findInt("u_tex2");
    Coel::Texture texture2("Assets/ColorGrid.png");

    while (window.isOpen()) {
        Coel::Renderer::clearColor();

        shader.bind();

        shader.send(u_tex1, 0);
        texture1.bind(0);
        shader.send(u_tex2, 1);
        texture2.bind(1);

        vao.drawIndexed(6);

        window.update();
    }
}
