#include <Coel.hpp>
#include <iostream>
#include <vector>

namespace Game {
    glm::ivec2 tileSize(16, 16);
    glm::vec2 tileCount{1, 1};

    struct Chunk {
        struct Element {
            enum ID {
                Air,
                Block,
            };
            unsigned char id;
        };
        glm::ivec2 pos;
        static inline constexpr glm::ivec2 SIZE{4, 4};
        Element elems[SIZE.y][SIZE.x];

        Chunk(const glm::ivec2 &pos) : pos(pos) {
            for (int yi = 0; yi < SIZE.y; ++yi) {
                for (int xi = 0; xi < SIZE.x; ++xi) {
                    auto p = glm::vec2(glm::ivec2(xi, yi) + pos * SIZE) / 50.f;
                    elems[yi][xi] = {(unsigned char)(glm::simplex(p) + glm::sqrt(p.y * 2) + 0.4)};
                }
            }
        }
    };
    std::vector<Chunk> chunks;

    struct Camera {
        glm::vec2 pos{0, 0};
    } cam;

    static struct Player {
        glm::vec2 pos{0, 0}, vel{0, 0};
        unsigned int moveNX : 1, movePX : 1, moveNY : 1, movePY : 1, isOnGround : 1;
        float speed{3.f};
        Player() : moveNX(false), movePX(false), moveNY(false), movePY(false), isOnGround(false) {}

        void update(const float elapsed) {
            if (movePX) vel.x = speed;
            if (moveNX) vel.x = -speed;
            if (movePY) vel.y = speed;
            if (moveNY) vel.y = -speed;

            pos += vel * elapsed;
            cam.pos = pos;
        }

        void move(const Coel::KeyInfo &key) {
            if (key.action == Coel::Action::Press) {
                switch (key.code) {
                case 'W': moveNY = true; break;
                case 'A': moveNX = true; break;
                case 'S': movePY = true; break;
                case 'D': movePX = true; break;
                default: break;
                }
            } else if (key.action == Coel::Action::Release) {
                switch (key.code) {
                case 'W': moveNY = false; break;
                case 'A': moveNX = false; break;
                case 'S': movePY = false; break;
                case 'D': movePX = false; break;
                default: break;
                }
            }
        }
    } player;
} // namespace Game

int main() {
    Coel::Window window(1280, 900, "Platformer");

    window.onKey = [](Coel::Window &w) { Game::player.move(w.key); };

    for (int yi = 0; yi < 10; ++yi)
        for (int xi = 0; xi < 10; ++xi)
            Game::chunks.push_back({{xi, yi}});

    Coel::Renderer::Batch2d batch;
    Coel::Renderer::enableBlend(true);
    Coel::Renderer::setClearColor(0.4, 0.4, 0.6f, 1.f);

    auto time = window.getTime(), prevTime = time, elapsed = time;
    while (window.isOpen()) {
        prevTime = time, time = window.getTime();
        elapsed = time - prevTime;
        Game::tileCount = glm::vec2(window.size) / glm::vec2(Game::tileSize);
        Game::player.update(elapsed);
        batch.resize({0, 0}, Game::tileCount);
        Coel::Renderer::clearColor();

        batch.begin();
        batch.fill({0, 0, 0, 255});
        for (const auto &chunk : Game::chunks) {
            unsigned int elemY = 0;
            for (const auto &elemRow : chunk.elems) {
                unsigned int elemX = 0;
                for (const auto &elem : elemRow) {
                    glm::vec2 elemPos{chunk.pos.x * chunk.SIZE.x + elemX, chunk.pos.y * chunk.SIZE.y + elemY};
                    if (elem.id) batch.submitRect(elemPos.x - Game::cam.pos.x, elemPos.y - Game::cam.pos.y, 1, 1);
                    ++elemX;
                }
                ++elemY;
            }
        }
        batch.submitRect(0 - Game::cam.pos.x, 0 - Game::cam.pos.y, 1, 1);
        batch.fill({0, 0, 0, 255});
        batch.submitRect(0 - Game::cam.pos.x, 0 - Game::cam.pos.y, 1, 1);
        batch.flush();

        window.update();
    }
}
