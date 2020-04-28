#include <Coel.hpp>
#include <iostream>

int main() {
    Coel::Window window{"Simple Model Loading Example"};
    Coel::create(window);

    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec3 a_pos;
    layout (location = 1) in vec3 a_nrm;
    layout (location = 2) in vec2 a_tex;
    out vec3 v_nrm;
    out vec2 v_tex;
    uniform mat4 u_projMat;
    uniform mat4 u_viewMat;
    uniform mat4 u_modlMat;
    void main() {
        vec4 worldPos = u_modlMat * vec4(a_pos, 1);
        v_nrm =  (u_modlMat * vec4(a_nrm, 1)).xyz;
        v_tex = a_tex;
        gl_Position = u_projMat * u_viewMat * worldPos;
    })";

    const char *const fragSrc = R"(
    #version 450 core
    in vec3 v_nrm;
    in vec2 v_tex;
    out vec4 frag_color;
    uniform sampler2D u_textures[32];
    void main() {
        vec4 color = texture(u_textures[0], v_tex);
        // frag_color = vec4(v_nrm, 1);
        frag_color = color;
    })";

    Coel::Shader shader;
    Coel::create(shader, vertSrc, fragSrc);
    auto u_projMat = Coel::findMat4(shader, "u_projMat");
    auto u_viewMat = Coel::findMat4(shader, "u_viewMat");
    auto u_modlMat = Coel::findMat4(shader, "u_modlMat");
    glm::mat4 projMat{1}, viewMat{1}, modlMat{1};
    Coel::Model model;
    Coel::open(model, "Assets/Objects/nanosuit/nanosuit.obj");
    Coel::Renderer::enableDepthTest(true);
    Coel::Renderer::enableCulling(false);
    Coel::Renderer::setClearColor(0.2f, 0.2f, 0.3f, 1.0f);

    while (window.isOpen) {
        Coel::Renderer::clear();
        Coel::bind(shader);
        projMat = glm::perspective(glm::radians(45.f), (float)window.size.x / window.size.y, 0.01f, 100.f);
        viewMat = glm::translate(glm::identity<glm::mat4>(), {0, -8, -25});
        modlMat = glm::rotate(glm::identity<glm::mat4>(), (float)Coel::getTime(), {0, 1, 0});
        Coel::send(u_projMat, &projMat);
        Coel::send(u_viewMat, &viewMat);
        Coel::send(u_modlMat, &modlMat);
        Coel::Renderer::draw(model);
        Coel::update(window);
    }
}
