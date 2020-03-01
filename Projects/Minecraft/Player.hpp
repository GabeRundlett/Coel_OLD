#pragma once

#include <Coel.hpp>

namespace Minecraft {
    struct Player {
        Math::Vec2d mouseCenter{};
        Math::Vec3 pos{}, vel{}, rot{};
        Coel::Camera3D cam;

        unsigned int shouldMoveForward : 1, shouldMoveBackward : 1, shouldMoveLeft : 1, shouldMoveRight : 1, shouldMoveUp : 1,
            shouldMoveDown : 1, shouldJump : 1, shouldSprint : 1, shouldSneak : 1, isFalling : 1;

        float sneakMult = 0.2f, sprintMult = 1.5f, height = 1.8f;
        float mouseSensitivity = 0.01f, movementSpeed = 100.f;

        void keyUpdate(const Coel::KeyInfo &kInfo);
        void mouseUpdate(const Coel::MouseInfo &mInfo);
        void windowSizeUpdate(const Math::Vec2i &size);

        void update(const float elapsed);

        void setPos(const Math::Vec3 &p);
        void setRot(const Math::Vec3 &r);
    };
} // namespace Minecraft
