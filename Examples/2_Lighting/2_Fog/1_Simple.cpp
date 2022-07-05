#include <Coel.hpp>
#include <iostream>

int main() {
    Coel::Window window({800, 600}, "Combined Phong Lighting Example");
    Coel::create(window);

    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec3 a_pos;
    layout (location = 1) in vec3 a_nrm;
    out vec3 v_col;
    out vec3 v_nrm;
    out vec3 v_tol;
    out vec3 v_rfl;

    const float density = 0.05, gradient = 15;
    out float v_visibility;
    
    uniform mat4 u_projMat;
    uniform mat4 u_viewMat;
    uniform mat4 u_modlMat;
    uniform mat4 u_modlNrmMat;
    
    void main() {
        vec3 lightPos = vec3(0, 10, 8);
        v_col = vec3(1, 0, 0);
        vec4 worldPos = u_modlMat * vec4(a_pos, 1);

        vec4 viewPos = u_viewMat * worldPos;
        float viewDist = -length(viewPos.xyz);
        v_visibility = clamp(exp(-pow(density * viewDist, -gradient)), 0, 1);
        
        v_nrm =  (u_modlMat * vec4(a_nrm, 1)).xyz;
        v_tol = normalize(lightPos - worldPos.xyz);
        v_rfl = reflect(-v_tol, v_nrm);
        gl_Position = u_projMat * viewPos;
    }
    )";

    const char *const fragSrc = R"(
    #version 450 core
    in vec3 v_col;
    in vec3 v_nrm;
    in vec3 v_tol;
    in vec3 v_rfl;

    in float v_visibility;
    uniform vec3 u_fogColor;

    out vec4 frag_color;
    void main() {
        vec3 lightCol = vec3(1);
        float diff = max(dot(v_tol, v_nrm), 0);
        float spec = max(pow(dot(v_rfl, v_nrm), 16), 0);
        frag_color = vec4(mix(lightCol * (v_col * diff + spec), u_fogColor, v_visibility), 1);
    }
    )";

    Coel::Shader shader;
    Coel::create(shader, vertSrc, fragSrc);

    auto u_projMat = Coel::findMat4(shader, "u_projMat");
    auto u_viewMat = Coel::findMat4(shader, "u_viewMat");
    auto u_modlMat = Coel::findMat4(shader, "u_modlMat");
    auto u_modlNrmMat = Coel::findMat4(shader, "u_modlNrmMat");
    glm::mat4 projMat, viewMat;

    auto u_fogColor = Coel::findFloat3(shader, "u_fogColor");
    glm::vec3 fogColor{0.6, 0.6, 0.8};

    Coel::Model model;
    Coel::open(model, "Assets/Objects/frog/scene.gltf");

    Coel::Renderer::enableCulling(true);
    Coel::Renderer::enableDepthTest(true);
    Coel::Renderer::setClearColor(fogColor.r, fogColor.g, fogColor.b, 1);

    while (window.isOpen) {
        Coel::Renderer::clear();

        Coel::bind(shader);

        projMat = glm::perspective(glm::radians(45.f), static_cast<float>(window.size.x) / static_cast<float>(window.size.y), 0.01f, 100.f);
        viewMat = glm::translate(glm::identity<glm::mat4>(), {0, -5, -18});

        Coel::send(u_projMat, &projMat);
        Coel::send(u_viewMat, &viewMat);
        Coel::send(u_fogColor, &fogColor);

        Coel::Renderer::draw(model, u_modlMat, u_modlNrmMat);

        Coel::update(window);
    }
}
