#include <Coel.hpp>
#include <iostream>

constexpr unsigned int TILE_MAP_SIZE_X = 22, TILE_MAP_SIZE_Y = 8;
const char *tiles[TILE_MAP_SIZE_Y] = {
    "#....................#", //
    "#....................#", //
    "#....................#", //
    "#............#######.#", //
    "###......##..#.......#", //
    ".......#.....#.#######", //
    "#...#..#.....#.......#", //
    "######################", //
};
const glm::vec2 tileSize{150, 150};

struct Camera {
    glm::vec2 pos{0, 0};
    void update(const glm::ivec2 &windowSize) {
        if (pos.x < 0) pos.x = 0;
        if (pos.y < 0) pos.y = 0;
        if (pos.x > tileSize.x * TILE_MAP_SIZE_X - windowSize.x) pos.x = tileSize.x * TILE_MAP_SIZE_X - windowSize.x;
        if (pos.y > tileSize.y * TILE_MAP_SIZE_Y - windowSize.y) pos.y = tileSize.y * TILE_MAP_SIZE_Y - windowSize.y;
    }
};

struct Player {
    glm::vec2 pos{2.f * tileSize.x, 5.5f * tileSize.y}, vel{0, 0};
    Camera cam{};
    unsigned int moveNX : 1, movePX : 1, moveNY : 1, movePY : 1, isOnGround : 1;
    Player() : moveNX(false), movePX(false), moveNY(false), movePY(false), isOnGround(false) {}
    void update(const float elapsed, const glm::ivec2 &windowSize) {
        constexpr float speed = 3;
        vel.y += 9.8 * tileSize.y * elapsed;
        if (movePX) vel.x = tileSize.x * speed;
        if (moveNX) vel.x = -tileSize.x * speed;
        if (moveNY && isOnGround) vel.y = -1.5 * tileSize.y * speed;
        isOnGround = false;
        auto newPos = pos + vel * elapsed;

        int nxi = newPos.x / tileSize.x, nyi = newPos.y / tileSize.y;

        int yi1 = pos.y / tileSize.y + 0.1, yi9 = pos.y / tileSize.y + 0.9;
        int xi1 = pos.x / tileSize.x + 0.1, xi9 = pos.x / tileSize.x + 0.9;

        if (nxi > -1 && nxi < TILE_MAP_SIZE_X - 1 && yi1 > -1 && yi9 < TILE_MAP_SIZE_Y) {
            if (vel.x < 0.f) {
                if (tiles[yi1][nxi] != '.' || tiles[yi9][nxi] != '.') newPos.x = tileSize.x * (nxi + 1), vel.x = 0;
            } else {
                if (tiles[yi1][nxi + 1] != '.' || tiles[yi9][nxi + 1] != '.') newPos.x = tileSize.x * nxi, vel.x = 0;
            }
        }

        if (xi1 > -1 && xi9 < TILE_MAP_SIZE_X && nyi > -1 && nyi < TILE_MAP_SIZE_Y - 1) {
            if (vel.y < 0.f) {
                if (tiles[nyi][xi1] != '.' || tiles[nyi][xi9] != '.') newPos.y = tileSize.y * (nyi + 1), vel.y = 0;
            } else {
                if (tiles[nyi + 1][xi1] != '.' || tiles[nyi + 1][xi9] != '.')
                    newPos.y = tileSize.y * nyi, vel.y = 0, isOnGround = true;
            }
        }

        pos = newPos;
        // Put cam position offset to the *center of the screen
        cam.pos = pos - tileSize * glm::vec2{4, 2};
        cam.update(windowSize);
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

    const char *const vertSrc = R"(
    #version 450 core
    layout (location = 0) in vec2 a_pos;
    layout (location = 1) in vec2 a_tex;
    layout (location = 2) in vec2 a_size;
    layout (location = 3) in vec4 a_col;
    uniform mat4 u_projMat;
    uniform mat4 u_viewMat;
    uniform mat4 u_modlMat;
    out vec4 v_col;
    void main() {
        v_col = a_col;
        vec4 worldPos = u_modlMat * vec4(a_pos, 0, 1);
        gl_Position = u_projMat * u_viewMat * worldPos;
    }
    )";

    const char *const fragSrc = R"(
    #version 450 core
    in vec4 v_col;
    out vec4 frag_color;
    void main() {
        frag_color = v_col;
    }
    )";

    Coel::Shader shader(vertSrc, fragSrc);

    Coel::Renderer::setClearColor(0.8, 0.82, 1.f, 1.f);
    Coel::Renderer::Batch2d batch;

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
        for (unsigned int yi = 0; yi < TILE_MAP_SIZE_Y; ++yi) {
            const auto &tile_x = tiles[yi];
            for (unsigned int xi = 0; xi < TILE_MAP_SIZE_X; ++xi) {
                const auto &tile = tile_x[xi];
                if (tile == '#') { //
                    batch.submitRect(xi * tileSize.x - player.cam.pos.x, yi * tileSize.y - player.cam.pos.y, tileSize.x,
                                     tileSize.y);
                }
            }
        }

        batch.fill({0, 150, 120, 255});
        if (player.isOnGround) batch.fill({255, 150, 120, 255});
        batch.submitRect(player.pos.x - player.cam.pos.x, player.pos.y - player.cam.pos.y, tileSize.x, tileSize.y);
        batch.flush();

        window.update();
    }
}
