#pragma once
#include <Coel.hpp>

#include "../../5_Model/0_Player.hpp"

namespace Scene {
    Coel::Shader shader;
    Coel::Uniform<glm::mat4> u_projMat, u_viewMat, u_modlMat, u_modlNrmMat;
    Coel::Uniform<int> u_tex, u_nrm_tex;
    glm::mat4 projMat{1}, viewMat{1};
    struct Object {
        Coel::Model model;
    };
    std::vector<Object> objects;

    Player player{};
    Camera camera{};
    bool is_paused = true;

    void toggle_pause(Coel::Window &window) {
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
    }

    void init(const char *const vertSrc, const char *const fragSrc, Coel::Window &window) {
        player.pos = {-1.75, -2.5, -3};
        player.rot = {glm::radians(-30.0f), glm::radians(90 - 65.0f), 0.0f};

        toggle_pause(window);

        window.onKey = [&](Coel::Window &window) {
            if (window.key.code == input::keybinds::TOGGLE_PAUSE && window.key.action == GLFW_PRESS)
                toggle_pause(window);
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

        Coel::create(shader, vertSrc, fragSrc);
        u_projMat = Coel::findMat4(shader, "u_projMat");
        u_viewMat = Coel::findMat4(shader, "u_viewMat");
        u_modlMat = Coel::findMat4(shader, "u_modlMat");
        u_modlNrmMat = Coel::findMat4(shader, "u_modlNrmMat");
        u_tex = Coel::findInt(shader, "u_tex");
        u_nrm_tex = Coel::findInt(shader, "u_nrm_tex");

        objects.push_back({});
        Coel::open(objects.back().model, "Assets/Objects/frog/scene.gltf");
        for (auto &o : objects.back().model.objects)
            o.modlMat *= glm::scale(glm::rotate(glm::translate(glm::mat4(1), {0, 0.4f, 0}), 0.25f, {1, 0, 0}), {1, 1, 1});
    }

    void draw(Coel::Window &window) {
        Coel::Renderer::enableDepthTest(true);
        Coel::Renderer::enableCulling(true);
        Coel::Renderer::setClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        Coel::Renderer::setClearDepth(1.0f);
        Coel::Renderer::clear();
        player.update(static_cast<float>(window.elapsed));
        camera.fov = player.fov;
        camera.resize(window.size.x, window.size.y);
        camera.set_pos(player.pos);
        camera.set_rot(player.rot.x, player.rot.y);
        projMat = camera.proj_mat;
        viewMat = camera.vrot_mat * camera.vtrn_mat;

        Coel::send(u_projMat, &projMat);
        Coel::send(u_viewMat, &viewMat);
        for (auto &o : objects)
            Coel::Renderer::draw(o.model, u_modlMat, u_modlNrmMat, u_tex, u_nrm_tex);
    }
} // namespace Scene