#include <Coel.hpp>
#include <iostream>

int main() {
    Coel::Window window({1280, 960}, "Specular Phong Lighting Example");
    Coel::create(window);

    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec3 a_pos;
    layout (location = 1) in vec3 a_nrm;
    out vec3 v_nrm;
    out vec3 v_rfl;
    uniform mat4 u_projMat;
    uniform mat4 u_viewMat;
    uniform mat4 u_modlMat;
    void main() {
        vec3 lightPos = vec3(0, 10, 8);
        vec4 worldPos = u_modlMat * vec4(a_pos, 1);
        v_nrm =  (u_modlMat * vec4(a_nrm, 1)).xyz;
        vec3 v_tol = normalize(lightPos - worldPos.xyz);
        v_rfl = reflect(-v_tol, v_nrm);
        gl_Position = u_projMat * u_viewMat * worldPos;
    }
    )";

    const char *const fragSrc = R"(
    #version 450 core
    in vec3 v_nrm;
    in vec3 v_rfl;
    out vec4 frag_color;
    void main() {
        vec3 lightCol = vec3(1);
        float spec = max(pow(dot(v_rfl, v_nrm), 16), 0);
        frag_color = vec4(lightCol * spec, 1);
    }
    )";

    Coel::Shader shader;
    Coel::create(shader, vertSrc, fragSrc);

    auto u_projMat = shader.findMat4("u_projMat");
    auto u_viewMat = shader.findMat4("u_viewMat");
    auto u_modlMat = shader.findMat4("u_modlMat");
    glm::mat4 projMat, viewMat, modlMat;

    Coel::Model model("Assets/dragon.obj");

    Coel::Renderer::enableCulling(true);
    Coel::Renderer::enableDepthTest(true);
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

        model.draw();

        window.update();
    }
}
