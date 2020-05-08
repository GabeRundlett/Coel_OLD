#pragma once
#include <Coel.hpp>

template <typename T> struct Animatable {
    T base, low_bound, high_bound;
    double speed = 1.f, val = (base - low_bound) / (high_bound - low_bound);
    int modifier = 0;

    inline void set_bounds(const T l, const T h) {
        low_bound = l, high_bound = h; //
    }

    void update(const float elapsed) {
        val += elapsed * speed * modifier; //
        if (val > 1) val = 1, modifier = 0;
        if (val < 0) val = 0, modifier = 0;
    }

    inline void play_forward() { modifier = 1; }
    inline void play_reverse() { modifier = -1; }

    inline T get() {
        auto width = high_bound - low_bound;
        return static_cast<T>(val * width + low_bound);
    }

    void reset() { val = base; }
};

struct Player {
    glm::dvec2 mouseCenter{};
    glm::vec3 pos{}, vel{}, rot{};
    Coel::Camera3D cam{};

    static inline constexpr bool UPDATE_POSITION = true, UPDATE_ROTATION = true;

    unsigned int shouldMoveForward : 1, shouldMoveBackward : 1, shouldMoveLeft : 1, shouldMoveRight : 1, shouldMoveUp : 1,
        shouldMoveDown : 1, shouldJump : 1, shouldSprint : 1, shouldSneak : 1, isFalling : 1;

    float sneakMult = 0.2f, sprintMult = 1.5f;
    float mouseSensitivity = 0.0001f, movementSpeed = 1000.f;

    Animatable<float> fov{cam.fov, cam.fov, cam.fov * 1.05f, 10};
    Animatable<float> height{1.8, 1.2, 1.8, 10};

    Player()
        : shouldMoveForward(false), shouldMoveBackward(false), shouldMoveLeft(false), shouldMoveRight(false),
          shouldMoveUp(false), shouldMoveDown(false), shouldJump(false), shouldSprint(false), shouldSneak(false),
          isFalling(false) {}

    static inline constexpr Coel::Key::KeyboardKeyID //
        keycodeForward = Coel::Key::W,               //
        keycodeLeft = Coel::Key::A,                  //
        keycodeBackward = Coel::Key::S,              //
        keycodeRight = Coel::Key::D,                 //
        keycodeJump = Coel::Key::Space,              //
        keycodeSprint = Coel::Key::LeftShift,        //
        keycodeSneak = Coel::Key::LeftControl        //
        ;

    void keyUpdate(const Coel::KeyInfo &kInfo) {
        switch (kInfo.action) {
        case Coel::Action::Press:
            switch (kInfo.code) {
            case keycodeForward: shouldMoveForward = true; break;
            case keycodeBackward: shouldMoveBackward = true; break;
            case keycodeLeft: shouldMoveLeft = true; break;
            case keycodeRight: shouldMoveRight = true; break;
            case keycodeJump: shouldJump = true; break;
            case keycodeSprint:
                shouldSprint = true;
                fov.play_forward();
                break;
            case keycodeSneak:
                shouldSneak = true;
                height.play_reverse();
                break;
            default: break;
            }
            break;
        case Coel::Action::Release:
            switch (kInfo.code) {
            case keycodeForward: shouldMoveForward = false; break;
            case keycodeBackward: shouldMoveBackward = false; break;
            case keycodeLeft: shouldMoveLeft = false; break;
            case keycodeRight: shouldMoveRight = false; break;
            case keycodeJump: shouldJump = false; break;
            case keycodeSprint:
                shouldSprint = false;
                fov.play_reverse();
                break;
            case keycodeSneak:
                shouldSneak = false;
                height.play_forward();
                break;
            default: break;
            }
            break;
        default: break;
        }
    }
    void mouseButtonUpdate(const Coel::MouseInfo &mInfo) {}
    void mouseScrollUpdate(const Coel::MouseInfo &mInfo) {
        fov.base = cam.fov - 0.01f * (float)mInfo.scrollOffset.y;
        fov.set_bounds(cam.fov, cam.fov * 1.25);
        cam.updateFov(fov.base);
    }
    void mouseMoveUpdate(const Coel::MouseInfo &mInfo) {
        auto offset = (mInfo.pos - mouseCenter) * (double)mouseSensitivity;
        if (UPDATE_ROTATION) {
            rot.x -= (float)offset.y * fov.base, rot.y -= (float)offset.x * fov.base;
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

            if (pos.y > -terrainHeight) {
                pos.y = -terrainHeight, vel.y = 0;
                isFalling = false;
            }
        }
        fov.update(elapsed);
        height.update(elapsed);

        auto temp_pos = pos;
        temp_pos.y -= height.get();

        cam.updatePosition(temp_pos);
        cam.updateFov(fov.get());
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
