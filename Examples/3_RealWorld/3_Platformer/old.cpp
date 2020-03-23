#include <Coel.hpp>
#include <iostream>
#include <vector>

#include <glm/gtx/string_cast.hpp>

struct Chunk {
    struct Element {
        enum ID {
            Air,
            Block,
        };
        unsigned char id;
    };
    glm::ivec2 pos;
    static inline constexpr glm::ivec2 SIZE{16, 16};
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

const glm::vec2 tileSize{16, 16};

struct Camera {
    glm::vec2 pos{0, 0};
};

std::vector<Chunk> chunks;

struct Player {
    glm::vec2 pos{tileSize}, vel{0, 0};
    Camera cam{};
    unsigned int moveNX : 1, movePX : 1, moveNY : 1, movePY : 1, isOnGround : 1;

    Player() : moveNX(false), movePX(false), moveNY(false), movePY(false), isOnGround(false) {}

    void update(const float elapsed, const glm::ivec2 &windowSize) {
        constexpr float speed = 6;
        vel.y += 9.8 * tileSize.y * elapsed;
        if (movePX) vel.x = tileSize.x * speed;
        if (moveNX) vel.x = -tileSize.x * speed;
        if (moveNY && isOnGround) vel.y = -1.5 * tileSize.y * speed;
        isOnGround = false;
        auto newPos = pos + vel * elapsed;

        auto ni = glm::ivec2(newPos / tileSize), p1 = glm::ivec2(pos / tileSize + glm::vec2(0.1, 0.1)),
             p9 = glm::ivec2(pos / tileSize + glm::vec2(0.9, 0.9));
        auto newPosChunk = ni / Chunk::SIZE;

        // if (newPos.x < 0) ni.x -= 1;
        // if (newPos.y < 0) ni.y -= 1;

        // if (pos.x < 0) p1.x -= 1, p9.x -= 1;
        // if (pos.y < 0) p1.y -= 1, p9.y -= 1;

        // if (ni.x < 0) newPosChunk.x -= 1;
        // if (ni.y < 0) newPosChunk.y -= 1;

        bool checkExtraX = ni.x % Chunk::SIZE.x == Chunk::SIZE.x - 1;
        bool checkExtraY = ni.y % Chunk::SIZE.y == Chunk::SIZE.y - 1;

        const auto collideChunk = [&ni, &p1, &p9, &newPos](const Chunk &chunk, Player &p) {
            const auto ind = glm::ivec2(ni.x % Chunk::SIZE.x, ni.y % Chunk::SIZE.y);
            const auto ind1 = glm::ivec2(p1.x % Chunk::SIZE.x, p1.y % Chunk::SIZE.y);
            const auto ind9 = glm::ivec2(p9.x % Chunk::SIZE.x, p9.y % Chunk::SIZE.y);
            if (ind.x > -1 && ind.x < Chunk::SIZE.x - 1 && ind1.y > -1 && ind9.y < Chunk::SIZE.y) {
                if (p.vel.x < 0.f) {
                    if (chunk.elems[ind1.y][ind.x].id != Chunk::Element::Air ||
                        chunk.elems[ind9.y][ind.x].id != Chunk::Element::Air)
                        newPos.x = tileSize.x * (ind.x + 1 + chunk.pos.x * Chunk::SIZE.x), p.vel.x = 0;
                } else {
                    if (chunk.elems[ind1.y][ind.x + 1].id != Chunk::Element::Air ||
                        chunk.elems[ind9.y][ind.x + 1].id != Chunk::Element::Air)
                        newPos.x = tileSize.x * (ind.x + chunk.pos.x * Chunk::SIZE.x), p.vel.x = 0;
                }
            }

            if (ind1.x > -1 && ind9.x < Chunk::SIZE.x && ind.y > -1 && ind.y < Chunk::SIZE.y - 1) {
                if (p.vel.y < 0.f) {
                    if (chunk.elems[ind.y][ind1.x].id != Chunk::Element::Air ||
                        chunk.elems[ind.y][ind9.x].id != Chunk::Element::Air)
                        newPos.y = tileSize.y * (ind.y + 1 + chunk.pos.y * Chunk::SIZE.y), p.vel.y = 0;
                } else {
                    if (chunk.elems[ind.y + 1][ind1.x].id != Chunk::Element::Air ||
                        chunk.elems[ind.y + 1][ind9.x].id != Chunk::Element::Air)
                        newPos.y = tileSize.y * (ind.y + chunk.pos.y * Chunk::SIZE.y), p.vel.y = 0, p.isOnGround = true;
                }
            }
        };

        for (auto &c : chunks) {
            if (c.pos == newPosChunk) collideChunk(c, *this);
            if (checkExtraX) {
                if (c.pos + glm::ivec2(1, 0) == newPosChunk) collideChunk(c, *this);
                if (checkExtraY)
                    if (c.pos + glm::ivec2(1, 1) == newPosChunk) collideChunk(c, *this);
            }
            if (checkExtraY) {
                if (c.pos + glm::ivec2(0, 1) == newPosChunk) collideChunk(c, *this);
                if (checkExtraY)
                    if (c.pos + glm::ivec2(1, 1) == newPosChunk) collideChunk(c, *this);
            }
        }

        pos = newPos;
        // Put cam position offset to the *center of the screen
        cam.pos = pos - tileSize * glm::vec2{1280 / 64 / 2, 900 / 64 / 2};
    }
} player;

int main() {
    Coel::Window window(1280, 900, "Platformer");
    window.onKey = [](Coel::Window &w) {
        if (w.key.action == Coel::Action::Press) {
            switch (w.key.code) {
            case 'W': player.moveNY = true; break;
            case 'A': player.moveNX = true; break;
            case 'S': player.movePY = true; break;
            case 'D': player.movePX = true; break;
            default: break;
            }
        } else if (w.key.action == Coel::Action::Release) {
            switch (w.key.code) {
            case 'W': player.moveNY = false; break;
            case 'A': player.moveNX = false; break;
            case 'S': player.movePY = false; break;
            case 'D': player.movePX = false; break;
            default: break;
            }
        }
    };

    Coel::Renderer::setClearColor(0.8, 0.82, 1.f, 1.f);
    Coel::Renderer::Batch2d batch;
    Coel::Renderer::enableBlend(true);

    for (int yi = 0; yi < 10; ++yi)
        for (int xi = 0; xi < 10; ++xi)
            chunks.push_back({{xi, yi}});

    auto time = window.getTime(), prevTime = time, elapsed = time;
    while (window.isOpen()) {
        prevTime = time, time = window.getTime();
        elapsed = time - prevTime;

        const glm::vec2 tileCount{glm::vec2{window.size} / tileSize};

        Coel::Renderer::clearColor();
        batch.resize({0, 0}, window.size);

        player.update(elapsed, window.size);

        batch.begin();
        batch.fill({180, 120, 100, 255});
        for (const auto &chunk : chunks) {
            unsigned int elemY = 0;
            for (const auto &elemRow : chunk.elems) {
                unsigned int elemX = 0;
                for (const auto &elem : elemRow) {
                    glm::vec2 elemPos{tileSize.x * (chunk.pos.x * chunk.SIZE.x + elemX),
                                      tileSize.y * (chunk.pos.y * chunk.SIZE.y + elemY)};
                    if (elem.id)
                        batch.submitRect(elemPos.x - player.cam.pos.x, elemPos.y - player.cam.pos.y, tileSize.x, tileSize.y);
                    ++elemX;
                }
                ++elemY;
            }
        }

        batch.fill({0, 150, 120, 255});
        if (player.isOnGround) batch.fill({255, 150, 120, 255});
        batch.submitRect(player.pos.x - player.cam.pos.x, player.pos.y - player.cam.pos.y, tileSize.x, tileSize.y);
        batch.flush();

        window.update();
    }
}
