#include <Coel.hpp>
#include <iostream>

int main() {
    Coel::Window window(800, 600, "Simple Model Loading Example");

    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec3 a_pos;
    layout (location = 1) in vec3 a_nrm;
    layout (location = 2) in vec2 a_tex;
    out vec2 v_tex;
    uniform mat4 u_projMat;
    uniform mat4 u_viewMat;
    uniform mat4 u_modlMat;
    void main() {
        v_tex = a_tex;
        vec4 worldPos = u_modlMat * vec4(a_pos, 1);
        gl_Position = u_projMat * u_viewMat * worldPos;
    }
    )";

    const char *const fragSrc = R"(
    #version 450 core
    in vec2 v_tex;
    out vec4 frag_color;
    uniform sampler2D u_tex;
    void main() {
        frag_color = texture(u_tex, v_tex);
    }
    )";

    Coel::Shader shader(vertSrc, fragSrc);

    auto u_projMat = shader.findMat4("u_projMat");
    auto u_viewMat = shader.findMat4("u_viewMat");
    auto u_modlMat = shader.findMat4("u_modlMat");
    glm::mat4 projMat, viewMat, modlMat;

    Coel::Model model("Assets/stall.obj");
    auto u_tex = shader.findInt("u_tex");
    Coel::Texture texture("Assets/stall.png");

    Coel::Renderer::enableDepthTest(true);
    Coel::Renderer::enableCulling(true);
    Coel::Renderer::setClearColor(0.6, 0.6, 0.8, 1);

    while (window.isOpen()) {
        Coel::Renderer::clear();

        shader.bind();

        projMat = glm::perspective(glm::radians(45.f), (float)window.size.x / window.size.y, 0.01f, 100.f);
        viewMat = glm::translate(glm::identity<glm::mat4>(), {0, -5, -18});
        modlMat = glm::rotate(glm::identity<glm::mat4>(), (float)window.getTime(), {0, 1, 0});

        shader.send(u_projMat, &projMat);
        shader.send(u_viewMat, &viewMat);
        shader.send(u_modlMat, &modlMat);
        shader.send(u_tex, 0);
        texture.bind(0);

        model.draw();

        window.update();
    }
}
