#include <Coel.hpp>

static inline constexpr glm::vec4 toTexPosSize(const float x, const float y, const float w = 1, const float h = 1) {
    constexpr float TILE_COUNT = 4, FACTOR = 1.f / TILE_COUNT;
    constexpr float PADDING = FACTOR / 128;
    return {x * FACTOR + PADDING, y * FACTOR + PADDING, w * FACTOR - PADDING * 2, h * FACTOR - PADDING * 2};
}

struct Player {
    glm::vec2 pos, vel;

    unsigned int shouldMoveForward : 1, shouldMoveBackward : 1, shouldMoveLeft : 1, shouldMoveRight : 1, shouldMoveUp : 1,
        shouldMoveDown : 1;

    float speed = 50.0f;
    int facing = 1;
    bool moving = false;
    bool onGround = false;

    void onKey(Coel::KeyInfo &key) {
        switch (key.action) {
        case Coel::Action::Press:
            switch (key.code) {
            case Coel::Key::W: shouldMoveForward = true; break;
            case Coel::Key::S: shouldMoveBackward = true; break;
            case Coel::Key::A: shouldMoveLeft = true; break;
            case Coel::Key::D: shouldMoveRight = true; break;
            case Coel::Key::Space:
                shouldMoveUp = true;
                if (onGround) {
                    onGround = false;
                    vel.y = 12.0f;
                }
                break;
            case Coel::Key::LeftShift: shouldMoveDown = true; break;
            default: break;
            }
            break;
        case Coel::Action::Release:
            switch (key.code) {
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

    void onMouseMove(Coel::MouseInfo &) {
    }

    void update(float delta_time) {
        if (shouldMoveLeft) {
            if (onGround) {
                vel.x -= delta_time * speed;
                moving = true;
            }
            facing = -1;
        }
        if (shouldMoveRight) {
            if (onGround) {
                vel.x += delta_time * speed;
                moving = true;
            }
            facing = 1;
        }
        if (vel.x < -speed / 5)
            vel.x = -speed / 5;
        if (vel.x > speed / 5)
            vel.x = speed / 5;

        vel.y += -9.8f * delta_time * 3.0f;
        pos += vel * float(delta_time);

        moving = false;
        onGround = false;

        if (pos.y < 0) {
            pos.y = 0;
            onGround = true;
        }

        if (onGround) {
            vel.x *= (1.0f - delta_time * 10);
        }
    }
};

int main() {
    Coel::Window window({800, 800}, "Platformer Example");
    Coel::SpriteRenderer spriteRenderer;
    float aspect = 1, scale = 3;

    double player_sprite_anim_start = 0;
    Player player{.pos{}, .vel{}};

    // std::array<std::array<Tile, 5>, 1>

    window.onResize = [&aspect, &scale](Coel::Window &w) {
        aspect = static_cast<float>(w.size.x) / static_cast<float>(w.size.y) * scale;
    };

    window.onMouseScroll = [&scale](Coel::Window &w) {
        scale -= static_cast<float>(w.mouse.scrollOffset.y) / 5.0f;
        if (scale < 0.4f)
            scale = 0.4f;
    };

    window.onKey = [&player](Coel::Window &w) {
        player.onKey(w.key);
    };

    window.onMouseMove = [&player](Coel::Window &w) {
        player.onMouseMove(w.mouse);
    };

    Coel::create(window);
    Coel::init(spriteRenderer);
    Coel::Renderer::setClearColor(0.68f, 0.68f, 0.84f, 1.0f);
    Coel::Renderer::enableBlend(true);

    Coel::Texture spriteSheet;
    Coel::create(spriteSheet, "Assets/Textures/sheet.png");
    Coel::setMagFilter(spriteSheet, Coel::Nearest);
    Coel::setWrapMode(spriteSheet, Coel::ClampToEdge);

    while (window.isOpen) {
        Coel::Renderer::clearColor();

        bool prev_moving = player.moving;
        player.update(static_cast<float>(window.elapsed));
        if (prev_moving != player.moving)
            player_sprite_anim_start = window.time;

        // float off = player.pos.x;
        // float off1 = off / 1.0f - round(off) / 1.0f;
        // float off2 = off / 24.0f - round(off) / 24.0f;
        // float off3 = off / 12.0f - round(off) / 12.0f;

        aspect = static_cast<float>(window.size.x) / static_cast<float>(window.size.y) * scale;
        spriteRenderer.resize(aspect, scale);
        spriteRenderer.begin();
        // const float s4 = round(scale / 4.0f) + 3.0f;
        // const float s1 = round(scale) + 3.0f;
        // const glm::vec4 tint = {0.9, 0.9, 0.95, 1.0};

        // // hills
        // for (float x = -s4 * 4; x < s4 * 4; x += 4)
        //     spriteRenderer.submit({x - off3 * 4, 0, 4, 1}, toTexPosSize(0, 2, 4, 1), spriteSheet, tint);

        // // clouds
        // for (float x = -s4 * 4; x < s4 * 4; x += 4)
        //     spriteRenderer.submit({x - off2 * 4, 1, 4, 1}, toTexPosSize(0, 3, 4, 1), spriteSheet, tint);

        // // dirt
        // for (float y = -s1 * 4; y < 0; ++y)
        //     for (float x = -s1 * 1; x < s1 * 1; ++x)
        //         spriteRenderer.submit({x - off1, y, 1, 1}, toTexPosSize(0, 1), spriteSheet, tint);

        // // Blocks (dirt, grass)
        // spriteRenderer.submit({-off, 0, 1, 1}, toTexPosSize(0, 1), spriteSheet, tint);
        // // spriteRenderer.submit({-off, 0.25, 1, 1}, toTexPosSize(1, 1), spriteSheet, tint);

        // spriteRenderer.submit({-off, 1, 1, 1}, toTexPosSize(0, 1), spriteSheet, tint);
        // spriteRenderer.submit({-off, 1.25, 1, 1}, toTexPosSize(1, 1), spriteSheet, tint);

        // // grass
        // for (float x = -s1 * 1; x < s1 * 1; ++x)
        //     spriteRenderer.submit({x - off1, -0.75, 1, 1}, toTexPosSize(1, 1), spriteSheet, tint);

        // player
        int sprite_anim_offset = 0;
        if (player.shouldMoveLeft || player.shouldMoveRight)
            sprite_anim_offset = static_cast<int>((window.time - player_sprite_anim_start + 0.25) * 4.0) % 4;
        spriteRenderer.submit({0, player.pos.y, player.facing, 1}, toTexPosSize(static_cast<float>(sprite_anim_offset), 0.0f), spriteSheet);

        // tallgrass foreground
        // for (float x = -s1 * 1; x < s1 * 1; ++x)
        //     spriteRenderer.submit({x - off1, 0, 1, 1}, toTexPosSize(2, 1), spriteSheet, tint);

        spriteRenderer.flush();

        Coel::update(window);
    }

    Coel::deinit(spriteRenderer);
    Coel::destroy(window);
}
