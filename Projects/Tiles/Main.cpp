#include <Coel.hpp>
#include "Shaders.hpp"
#include <iostream>

#include "Noise.hpp"

Coel::Window window(1400, 900, "Tiles");

struct Vertex {
    Math::Vec2 pos, size;
};
struct Index {
    unsigned short id;
};
struct SpriteVertices {
    Vertex a, b, c, d;
};
struct SpriteIndices {
    Index data[6];
};

static constexpr unsigned int SPRITE_COUNT = 10000;
static unsigned int spriteCount = 0;

Coel::Vao vao;
Coel::Vbo vbo(nullptr, SPRITE_COUNT * sizeof(SpriteVertices), {{Coel::Element::F32, 2}, {Coel::Element::F32, 2}});
Coel::Ibo ibo(nullptr, SPRITE_COUNT * sizeof(SpriteIndices));

static SpriteVertices *spriteVertices = nullptr;
static SpriteIndices *spriteIndices = nullptr;

Coel::Shader shader(vertSrc, fragSrc);
Math::Mat4 viewMat{1};

Coel::Texture spriteAtlas("C:/Dev/Coel/Projects/Tiles/Assets/Atlas.png");

Math::Vec2 camPos = {0, 0};
float camSpeed = 1.5f;

float zoom = 10.f, aspect = 1.f;
double time = 0, prevTime = 0;

bool moveCamUp = false, moveCamDown = false, moveCamLeft = false, moveCamRight = false;
const auto upKey = Coel::Key::W, downKey = Coel::Key::S, leftKey = Coel::Key::A, rightKey = Coel::Key::D;

struct Tile {
    unsigned char id, var;
};
static constexpr unsigned int TILE_COUNT_X = 100, TILE_COUNT_Y = 100;
Tile tiles[TILE_COUNT_X][TILE_COUNT_Y];
Tile tileToBePlaced = {0, 0};
bool placeTile = false;

static inline void begin() {
    spriteCount = 0;

    vbo.open(&spriteVertices);
    ibo.open(&spriteIndices);

    shader.bind();
}

static inline void flush() {
    vbo.close();
    ibo.close();

    vao.drawIndexed(spriteCount * 6);

    begin();
}

struct SpriteProp {
    Math::Vec2 pos, size, texPos, texSize;
};

static inline void drawSprite(const SpriteProp &sprite) {
    if (spriteCount > SPRITE_COUNT - 1) flush();

    *spriteVertices = {
        {sprite.pos, sprite.texPos},
        {sprite.pos.x, sprite.pos.y + sprite.size.y, sprite.texPos.x, sprite.texPos.y + sprite.texSize.y},
        {sprite.pos.x + sprite.size.x, sprite.pos.y, sprite.texPos.x + sprite.texSize.x, sprite.texPos.y},
        {sprite.pos + sprite.size, sprite.texPos + sprite.texSize},
    };

    unsigned short offset = spriteCount * 4;

    *spriteIndices = {
        unsigned short(offset + 1), 
        unsigned short(offset + 2), 
        unsigned short(offset + 0), 
        unsigned short(offset + 1), 
        unsigned short(offset + 3), 
        unsigned short(offset + 2),
    };

    ++spriteVertices;
    ++spriteIndices;
    ++spriteCount;
}

float random() { return float(rand() % 1000) / 1000; }

Tile *tileFromMouse(const Coel::Window &w) {
    float xOff = 2.f * w.mouse.pos.x / w.size.x - 1.f;
    float yOff = 1.f - 2.f * w.mouse.pos.y / w.size.y;

    int xIndex = (xOff * aspect - camPos.x) * zoom + TILE_COUNT_X / 2;
    int yIndex = (yOff - camPos.y) * zoom + TILE_COUNT_Y / 2;

    if (xIndex < 0 || xIndex > TILE_COUNT_X - 1 || yIndex < 0 || yIndex > TILE_COUNT_Y - 1) return nullptr;

    return &tiles[xIndex][yIndex];
}

int main() {
    window.onResize = [](Coel::Window &w) {
        aspect = (float)w.size.x / w.size.y;
        viewMat = Math::Mat4::ortho(-aspect, aspect, -1, 1, -1, 1);
        shader.sendMat4("viewMat", &viewMat);
    };
    window.onResize(window);

    window.onMouseButton = [](Coel::Window &w) {
        switch (w.mouse.action) {
        case Coel::Action::Press:
            switch (w.mouse.button) {
            case Coel::Button::Left: {
                placeTile = true;
                auto *tile = tileFromMouse(w);
                if (tile) *tile = tileToBePlaced;
                break;
            }
            case Coel::Button::Right: {
                auto *tile = tileFromMouse(w);
                if (tile) {
                    tile->var++;
                    tile->var %= 3;
                }
                break;
            }
            case Coel::Button::Middle: {
                auto *tile = tileFromMouse(w);
                if (tile) tileToBePlaced = *tile;
                break;
            }
            default: break;
            }
            break;
        case Coel::Action::Release:
            switch (w.mouse.button) {
            case Coel::Button::Left:
                placeTile = false; 
                break;
            default: break;
            }
            break;
        default: break;
        }
    };

    window.onMouseMove = [](Coel::Window &w) {
        if (placeTile) {
            auto *tile = tileFromMouse(w);
            if (tile) *tile = tileToBePlaced;
        }
    };

    window.onKey = [](Coel::Window &w) {
        switch (w.key.action) {
        case Coel::Action::Press:
            switch (w.key.code) {
            case upKey: moveCamUp = true; break;
            case downKey: moveCamDown = true; break;
            case leftKey: moveCamLeft = true; break;
            case rightKey: moveCamRight = true; break;
            default: break;
            }
            break;
        case Coel::Action::Release:
            switch (w.key.code) {
            case upKey: moveCamUp = false; break;
            case downKey: moveCamDown = false; break;
            case leftKey: moveCamLeft = false; break;
            case rightKey: moveCamRight = false; break;
            default: break;
            }
            break;
            default: break;
        }
    };

    for (int yi = 0; yi < TILE_COUNT_Y; ++yi) {
        for (int xi = 0; xi < TILE_COUNT_X; ++xi) {
            auto gen = random();
            if (gen < 0.9)
                tiles[xi][yi].var = 0;
            else if (gen < 0.95)
                tiles[xi][yi].var = 1;
            else
                tiles[xi][yi].var = 2;
            
            gen = ValueNoise_2D(10.0 * xi, 10.0 * yi);
            if (gen < -0.05)
                tiles[xi][yi].id = 1;
            else if (gen < 0.05) {
                if (gen > 0.02) tiles[xi][yi].var = 2;
                tiles[xi][yi].id = 0;
            }
            else
                tiles[xi][yi].id = 2;
        }
    }

    vbo.bind();
    ibo.bind();
    vao.add(vbo);
    begin();

    spriteAtlas.bind(0);

    Coel::Renderer::Command::setClearColor(0.4f, 0.4f, 1.f, 1.f);

    static constexpr int TILE_PADDING = 2;

    while (window.isOpen()) {
        Coel::Renderer::Command::clear();

        prevTime = time;
        time = window.getTime();
        auto elapsed = time - prevTime;

        if (moveCamUp) camPos.y -= camSpeed * elapsed;
        if (moveCamDown) camPos.y += camSpeed * elapsed;
        if (moveCamLeft) camPos.x += camSpeed * elapsed;
        if (moveCamRight) camPos.x -= camSpeed * elapsed;

        for (int yi = -zoom - TILE_PADDING; yi < zoom + TILE_PADDING; ++yi) {
            for (int xi = -zoom * aspect - TILE_PADDING; xi < zoom * aspect + TILE_PADDING; ++xi) {
                float x = (float)xi / zoom + camPos.x;
                float y = (float)yi / zoom + camPos.y;

                float w = 1.f / zoom;
                float h = 1.f / zoom;

                int xOff = camPos.x * zoom;
                int yOff = camPos.y * zoom;

                int xIndex = xi + TILE_COUNT_X / 2 - xOff;
                int yIndex = yi + TILE_COUNT_Y / 2 - yOff;

                bool insideTileRange = true;
                if (xIndex < 0 || xIndex > TILE_COUNT_X - 1 || yIndex < 0 || yIndex > TILE_COUNT_Y - 1) insideTileRange = false;

                if (insideTileRange) {
                    auto &tile = tiles[xIndex][yIndex];
                    int varOff = tile.var, idOff = tile.id;

                    drawSprite({x - (float)xOff / zoom, y - (float)yOff / zoom, w, h, 0.25f * idOff, 0.25f * varOff, 0.25, 0.25});
                }
            }
        }

        flush();
        window.update();
    }
}
