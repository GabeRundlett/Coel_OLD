#include <Coel.hpp>

int main() {
    Coel::Window window{{512, 512}, "Multiple Textures Example"};
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
    })";

    const char *const fragSrc = R"(
    #version 450 core
    in vec2 v_tex;
    out vec4 frag_color;
    uniform sampler2D u_tex1;
    uniform sampler2D u_tex2;
    void main() {
        frag_color = (texture(u_tex1, v_tex) + texture(u_tex2, v_tex)) / 2;
    })";

    Coel::Shader shader;
    Coel::create(shader, vertSrc, fragSrc);
    auto u_tex1 = Coel::findInt(shader, "u_tex1");
    Coel::Texture texture1;
    Coel::create(texture1, "Assets/Textures/sheet.png");
    auto u_tex2 = Coel::findInt(shader, "u_tex2");
    Coel::Texture texture2;
    Coel::create(texture2, "Assets/Textures/player.png");

    while (window.isOpen) {
        Coel::Renderer::clearColor();
        Coel::bind(shader);
        Coel::send(u_tex1, 0);
        Coel::bind(texture1, 0);
        Coel::send(u_tex2, 1);
        Coel::bind(texture2, 1);
        Coel::Renderer::drawIndexed(vao, 6);
        Coel::update(window);
    }

    Coel::destroy(texture1);
    Coel::destroy(texture2);
    Coel::destroy(shader);
    Coel::destroy(ibo);
    Coel::destroy(vbo);
    Coel::destroy(vao);
    Coel::destroy(window);
}
