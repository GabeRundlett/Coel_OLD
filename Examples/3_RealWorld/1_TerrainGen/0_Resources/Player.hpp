#pragma once
#include <Coel.hpp>

struct Player {
    glm::dvec2 mouseCenter{};
    glm::vec3 pos{}, vel{}, rot{};
    Coel::Camera3D cam{};

    static inline constexpr bool UPDATE_POSITION = true, UPDATE_ROTATION = true;

    unsigned int shouldMoveForward : 1, shouldMoveBackward : 1, shouldMoveLeft : 1, shouldMoveRight : 1, shouldMoveUp : 1,
        shouldMoveDown : 1, shouldJump : 1, shouldSprint : 1, shouldSneak : 1, isFalling : 1;

    float sneakMult = 0.2f, sprintMult = 1.5f, height = 1.8f;
    float mouseSensitivity = 0.001f, movementSpeed = 100.f;

    void keyUpdate(const Coel::KeyInfo &kInfo) {
        switch (kInfo.action) {
        case Coel::Action::Press:
            switch (kInfo.code) {
            case Coel::Key::W: shouldMoveForward = true; break;
            case Coel::Key::S: shouldMoveBackward = true; break;
            case Coel::Key::A: shouldMoveLeft = true; break;
            case Coel::Key::D: shouldMoveRight = true; break;
            case Coel::Key::Space: shouldJump = true; break;
            case Coel::Key::LeftControl: shouldSprint = true; break;
            case Coel::Key::LeftShift: shouldSneak = true, height = 1.2f, pos.y += 0.6f; break;
            default: break;
            }
            break;
        case Coel::Action::Release:
            switch (kInfo.code) {
            case Coel::Key::W: shouldMoveForward = false; break;
            case Coel::Key::S: shouldMoveBackward = false; break;
            case Coel::Key::A: shouldMoveLeft = false; break;
            case Coel::Key::D: shouldMoveRight = false; break;
            case Coel::Key::Space: shouldJump = false; break;
            case Coel::Key::LeftControl: shouldSprint = false; break;
            case Coel::Key::LeftShift: shouldSneak = false, height = 1.8f, pos.y -= 0.6f; break;
            default: break;
            }
            break;
        default: break;
        }
    }
    void mouseButtonUpdate(const Coel::MouseInfo &mInfo) {}
    void mouseScrollUpdate(const Coel::MouseInfo &mInfo) { cam.updateFov(cam.fov - 0.01f * (float)mInfo.scrollOffset.y); }
    void mouseMoveUpdate(const Coel::MouseInfo &mInfo) {
        auto offset = (mInfo.pos - mouseCenter) * (double)mouseSensitivity;
        if (UPDATE_ROTATION) {
            rot.x -= (float)offset.y * cam.fov, rot.y -= (float)offset.x * cam.fov;
            if (rot.x < -glm::radians(90.f)) rot.x = -glm::radians(90.f);
            if (rot.x > glm::radians(90.f)) rot.x = glm::radians(90.f);
        }
        cam.updateRotation(rot);
    }
    void windowSizeUpdate(const glm::ivec2 &size) {
        mouseCenter = {0.5 * size.x, 0.5 * size.y};
        cam.updateAspect(size);
    }

    void update(const float elapsed, const float terrainHeight = 0.f) {
        if constexpr (UPDATE_POSITION) {
            auto acceleration = movementSpeed;
            if (shouldSneak)
                acceleration *= sneakMult;
            else if (shouldSprint)
                acceleration *= sprintMult;

            float verticalDrag = 0.1f, lateralDrag = 0.9f;
            vel.y *= 1.f - elapsed * verticalDrag;
            vel.x *= 1.f - elapsed * lateralDrag;
            vel.z *= 1.f - elapsed * lateralDrag;

            if (isFalling) {
                vel.y += elapsed * 35;

                acceleration *= 0.05f;
            } else {
                vel *= 0.9f;
                if (shouldMoveUp) vel.y -= acceleration * elapsed;
                if (shouldMoveDown) vel.y += acceleration * elapsed;
                if (shouldJump) isFalling = true, vel.y = -10;
            }

            isFalling = true;
            const auto c = std::cos(rot.y), s = std::sin(rot.y);
            auto speed = vel.length();

            if (shouldMoveForward) {
                vel.x += acceleration * elapsed * s;
                vel.z += acceleration * elapsed * c;
            }
            if (shouldMoveBackward) {
                vel.x -= acceleration * elapsed * s;
                vel.z -= acceleration * elapsed * c;
            }
            if (shouldMoveLeft) {
                vel.x += acceleration * elapsed * c;
                vel.z -= acceleration * elapsed * s;
            }
            if (shouldMoveRight) {
                vel.x -= acceleration * elapsed * c;
                vel.z += acceleration * elapsed * s;
            }

            pos += vel * elapsed;

            if (pos.y > -height - terrainHeight) {
                pos.y = -height - terrainHeight, vel.y = 0;
                isFalling = false;
            }
        }
        cam.updatePosition(pos);
    }

    void setPos(const glm::vec3 &p) {
        cam.updatePosition(p);
        pos = p;
    }
    void setRot(const glm::vec3 &r) {
        cam.updateRotation(r);
        rot = r;
    }
};
