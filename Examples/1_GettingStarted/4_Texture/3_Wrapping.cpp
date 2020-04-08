#include <Coel.hpp>

int main() {
    Coel::Window window{{512, 512}, "Textured Quad with Sample Wrapping Example"};
    Coel::create(window);

    Coel::Vao vao;
    Coel::create(vao);

    float vertex_data[]{-1, -1, 0, 0, -1, 1, 0, 1, 1, -1, 1, 0, 1, 1, 1, 1};
    Coel::Vbo vbo{{{Coel::F32, 2}, {Coel::F32, 2}}};
    Coel::create(vbo, vertex_data, sizeof(vertex_data));
    Coel::link(vao, vbo);

    unsigned int index_data[]{0, 1, 2, 1, 3, 2};
    Coel::Ibo ibo;
    Coel::create(ibo, index_data, sizeof(index_data));

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

    uniform sampler2D u_tex;
    
    void main() {
        frag_color = texture(u_tex, v_tex * 4);
    }
    )";

    Coel::Shader shader(vertSrc, fragSrc);
    auto u_tex = shader.findInt("u_tex");
    Coel::Texture texture("Assets/ColorGrid.png");

    texture.setMagFilter(Coel::Filter::Linear);
    texture.setMinFilter(Coel::Filter::Linear);
    texture.setWrap(Coel::Wrap::Repeat);

    while (window.isOpen) {
        Coel::Renderer::clearColor();
        shader.bind();
        shader.send(u_tex, 0);
        texture.bind(0);
        Coel::Renderer::drawIndexed(vao, 6);
        Coel::update(window);
    }
}
