#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

namespace input::keybinds {
    static constexpr auto MOVE_PZ = GLFW_KEY_W;
    static constexpr auto MOVE_NZ = GLFW_KEY_S;
    static constexpr auto MOVE_PX = GLFW_KEY_A;
    static constexpr auto MOVE_NX = GLFW_KEY_D;
    static constexpr auto MOVE_PY = GLFW_KEY_SPACE;
    static constexpr auto MOVE_NY = GLFW_KEY_LEFT_SHIFT;

    static constexpr auto TOGGLE_PAUSE = GLFW_KEY_ESCAPE;
}

struct Player {
    struct MoveFlags {
        uint8_t px : 1, py : 1, pz : 1, nx : 1, ny : 1, nz : 1;
    } move;
    glm::vec3 pos{0, 0, 0}, vel{0, 0, 0}, rot{0, 0, 0};
    float     sin_rot_x = 0, cos_rot_x = 1;
    float     speed = 15.0f, mouse_sens = 0.001f;
    float     fov         = 45.0f;
    float     elapsed     = 0.0f;
    bool      is_grounded = false;

    void update(float delta_time) {
        elapsed += delta_time;
        auto delta_pos = speed * delta_time;
        if (move.px) pos.z += sin_rot_x * delta_pos, pos.x += cos_rot_x * delta_pos;
        if (move.nx) pos.z -= sin_rot_x * delta_pos, pos.x -= cos_rot_x * delta_pos;
        if (move.pz) pos.x -= sin_rot_x * delta_pos, pos.z += cos_rot_x * delta_pos;
        if (move.nz) pos.x += sin_rot_x * delta_pos, pos.z -= cos_rot_x * delta_pos;
        if (move.py) pos.y -= delta_pos;
        if (move.ny) pos.y += delta_pos;

        constexpr auto MAX_ROT = 1.54f;
        if (rot.y > MAX_ROT) rot.y = MAX_ROT;
        if (rot.y < -MAX_ROT) rot.y = -MAX_ROT;
    }
    void on_key(int key, int action) {
        switch (key) {
        case input::keybinds::MOVE_PZ: move.pz = action != GLFW_RELEASE; break;
        case input::keybinds::MOVE_NZ: move.nz = action != GLFW_RELEASE; break;

        case input::keybinds::MOVE_PX: move.px = action != GLFW_RELEASE; break;
        case input::keybinds::MOVE_NX: move.nx = action != GLFW_RELEASE; break;

        case input::keybinds::MOVE_PY: move.py = action != GLFW_RELEASE; break;
        case input::keybinds::MOVE_NY: move.ny = action != GLFW_RELEASE; break;
        }
    }
    void on_mouse_move(double delta_x, double delta_y) {
        rot.x += static_cast<float>(delta_x) * mouse_sens;
        rot.y -= static_cast<float>(delta_y) * mouse_sens;
        sin_rot_x = std::sin(rot.x);
        cos_rot_x = std::cos(rot.x);
    }
};
struct Camera {
    float     fov = 170.0f, aspect = 1.0f;
    float     near = 0.01f, far = 1000.0f;
    glm::mat4 proj_mat{};
    glm::mat4 vtrn_mat{};
    glm::mat4 vrot_mat{};

    void resize(int size_x, int size_y) {
        aspect   = static_cast<float>(size_x) / static_cast<float>(size_y);
        proj_mat = glm::perspective(glm::radians(fov), aspect, near, far);
    }
    void set_pos(glm::vec3 pos) { vtrn_mat = glm::translate(glm::mat4(1), pos); }
    void set_rot(float x, float y) {
        vrot_mat = glm::rotate(glm::rotate(glm::mat4(1), y, {1, 0, 0}), x, {0, 1, 0});
    }
    glm::mat4 update() { return proj_mat * vrot_mat * vtrn_mat; }
};
