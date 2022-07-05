#include <Coel.hpp>
#include <iostream>

#include "0_Player.hpp"

int main() {
    Coel::Window window{"Simple Model Loading Example"};
    Coel::create(window);

    Player player{};
    player.pos = {-1.75, -2.5, -3};
    player.rot = {glm::radians(-30.0f), glm::radians(90 - 65.0f), 0.0f};
    Camera camera{};
    bool is_paused = true;

    auto toggle_pause = [&]() {
        double center_x = static_cast<double>(window.size.x) / 2;
        double center_y = static_cast<double>(window.size.y) / 2;
        Coel::cursorTo(window, {center_x, center_y});
        if (is_paused) {
            Coel::cursorMode(window, Coel::CursorMode::Hidden);
            is_paused = false;
        } else {
            Coel::cursorMode(window, Coel::CursorMode::Normal);
            is_paused = true;
        }
    };

    toggle_pause();

    window.onKey = [&](Coel::Window &window) {
        if (window.key.code == input::keybinds::TOGGLE_PAUSE && window.key.action == GLFW_PRESS)
            toggle_pause();
        if (!is_paused) {
            player.on_key(window.key.code, window.key.action);
        }
    };
    window.onMouseMove = [&](Coel::Window &window) {
        double center_x = static_cast<double>(window.size.x) / 2;
        double center_y = static_cast<double>(window.size.y) / 2;

        auto x = window.mouse.pos.x;
        auto y = window.mouse.pos.y;
        auto dx = x - center_x;
        auto dy = y - center_y;
        if (!is_paused) {
            player.on_mouse_move(dx, -dy);
            Coel::cursorTo(window, {center_x, center_y});
        }
    };

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
    uniform mat4 u_modlNrmMat;
    void main() {
        vec4 worldPos = u_modlMat * vec4(a_pos, 1);
        v_nrm = (u_modlNrmMat * vec4(a_nrm, 1)).xyz;
        v_tex = a_tex;
        gl_Position = u_projMat * u_viewMat * worldPos;
    })";

    const char *const fragSrc = R"(
    #version 450 core
    in vec3 v_nrm;
    in vec2 v_tex;
    out vec4 frag_color;
    void main() {
        frag_color = vec4(v_nrm), 1);
    })";

    Coel::Shader shader;
    Coel::create(shader, vertSrc, fragSrc);
    auto u_projMat = Coel::findMat4(shader, "u_projMat");
    auto u_viewMat = Coel::findMat4(shader, "u_viewMat");
    auto u_modlMat = Coel::findMat4(shader, "u_modlMat");
    auto u_modlNrmMat = Coel::findMat4(shader, "u_modlNrmMat");
    glm::mat4 projMat{1}, viewMat{1};
    Coel::Model model;
    Coel::open(model, "C:/users/gabe/Downloads/gonza/export/y-up/gonza.gltf");
    Coel::Renderer::enableDepthTest(true);
    Coel::Renderer::enableCulling(false);
    Coel::Renderer::setClearColor(0.2f, 0.2f, 0.3f, 1.0f);

    while (window.isOpen) {
        Coel::Renderer::clear();
        Coel::bind(shader);
        player.update(static_cast<float>(window.elapsed));
        camera.fov = player.fov;
        camera.resize(window.size.x, window.size.y);
        camera.set_pos(player.pos);
        camera.set_rot(player.rot.x, player.rot.y);
        projMat = camera.proj_mat;
        viewMat = camera.vrot_mat * camera.vtrn_mat;
        Coel::send(u_projMat, &projMat);
        Coel::send(u_viewMat, &viewMat);
        Coel::Renderer::draw(model, u_modlMat, u_modlNrmMat);
        Coel::update(window);
    }
}
