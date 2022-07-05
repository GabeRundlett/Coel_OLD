#pragma once

#include <Coel.hpp>

struct Player {
    float speed{3.f}, mouseSensitivity{0.0001f};
    Coel::Camera3D cam{};
    glm::vec3 pos{0, 0, 0}, vel{speed, speed, speed}, rot{0, 0, 0};
    glm::dvec2 mouseCenter{};

    unsigned int shouldMoveForward : 1, shouldMoveBackward : 1, shouldMoveLeft : 1, shouldMoveRight : 1, shouldMoveUp : 1,
        shouldMoveDown : 1;

    void init() {
        cam.updatePosition(pos);
        cam.updateRotation(rot);
        shouldMoveForward = false;
        shouldMoveBackward = false;
        shouldMoveLeft = false;
        shouldMoveRight = false;
        shouldMoveUp = false;
        shouldMoveDown = false;
    }

    void updateCamera(const Coel::Shader &shader, const glm::ivec2 size, const Coel::Uniform<glm::mat4> &u_proj,
                      const Coel::Uniform<glm::mat4> &u_view) {
        mouseCenter = {0.5 * size.x, 0.5 * size.y};
        cam.updateAspect(size);
        Coel::bind(shader);
        Coel::send(u_proj, &cam.projMat);
        Coel::send(u_view, &cam.viewMat);
    }

    void updateCamera(const Coel::Shader &shader, const Coel::Uniform<glm::mat4> &u_proj,
                      const Coel::Uniform<glm::mat4> &u_view) {

        Coel::bind(shader);
        Coel::send(u_proj, &cam.projMat);
        Coel::send(u_view, &cam.viewMat);
    }

    void onKey(const Coel::KeyInfo &kInfo) {
        switch (kInfo.action) {
        case Coel::Action::Press:
            switch (kInfo.code) {
            case Coel::Key::W: shouldMoveForward = true; break;
            case Coel::Key::S: shouldMoveBackward = true; break;
            case Coel::Key::A: shouldMoveLeft = true; break;
            case Coel::Key::D: shouldMoveRight = true; break;
            case Coel::Key::Space: shouldMoveUp = true; break;
            case Coel::Key::LeftShift: shouldMoveDown = true; break;
            default: break;
            }
            break;
        case Coel::Action::Release:
            switch (kInfo.code) {
            case Coel::Key::W: shouldMoveForward = false; break;
            case Coel::Key::S: shouldMoveBackward = false; break;
            case Coel::Key::A: shouldMoveLeft = false; break;
            case Coel::Key::D: shouldMoveRight = false; break;
            case Coel::Key::Space: shouldMoveUp = false; break;
            case Coel::Key::LeftShift: shouldMoveDown = false; break;
            default: break;
            }
            break;
        default: break;
        }
    }

    void onMouseMove(const Coel::MouseInfo &mInfo) {
        auto offset = (mInfo.pos - mouseCenter) * static_cast<double>(mouseSensitivity);
        rot.x -= static_cast<float>(offset.y) * cam.fov, rot.y -= static_cast<float>(offset.x) * cam.fov;
        if (rot.x < -glm::radians(90.f))
            rot.x = -glm::radians(90.f);
        if (rot.x > glm::radians(90.f))
            rot.x = glm::radians(90.f);
        cam.updateRotation(rot);
    }

    void update(const float elapsed) {
        const auto c = std::cos(cam.rot.y), s = std::sin(cam.rot.y);

        if (shouldMoveForward) {
            pos.x += vel.x * elapsed * s;
            pos.z += vel.z * elapsed * c;
        }
        if (shouldMoveBackward) {
            pos.x -= vel.x * elapsed * s;
            pos.z -= vel.z * elapsed * c;
        }
        if (shouldMoveLeft) {
            pos.x += vel.x * elapsed * c;
            pos.z -= vel.z * elapsed * s;
        }
        if (shouldMoveRight) {
            pos.x -= vel.x * elapsed * c;
            pos.z += vel.z * elapsed * s;
        }

        if (shouldMoveUp)
            pos.y -= vel.y * elapsed;
        if (shouldMoveDown)
            pos.y += vel.y * elapsed;

        cam.updatePosition(pos);
    }
};
