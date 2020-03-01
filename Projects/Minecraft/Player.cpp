#include "Player.hpp"

namespace Minecraft {
    void Player::keyUpdate(const Coel::KeyInfo &kInfo) {
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
    void Player::mouseUpdate(const Coel::MouseInfo &mInfo) {
        auto offset = (mInfo.pos - mouseCenter) * mouseSensitivity;
        rot.x -= (float)offset.y, rot.y -= (float)offset.x;
        if (rot.x < -Math::HALF_PI) rot.x = (float)-Math::HALF_PI;
        if (rot.x > Math::HALF_PI) rot.x = (float)Math::HALF_PI;
        cam.updateRotation(rot);
    }
    void Player::windowSizeUpdate(const Math::Vec2i &size) {
        mouseCenter = {0.5 * size.x, 0.5 * size.y};
        cam.resizeProjection(size);
    }
    void Player::update(const float elapsed) {
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

        if (pos.y > -height) {
            pos.y = -height, vel.y = 0;
            isFalling = false;
        }
        cam.updatePosition(pos);
    }

    void Player::setPos(const Math::Vec3 &p) {
        cam.updatePosition(p);
        pos = p;
    }
    void Player::setRot(const Math::Vec3 &r) {
        cam.updateRotation(r);
        rot = r;
    }
} // namespace Minecraft
