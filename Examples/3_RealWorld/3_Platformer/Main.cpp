#include <Coel.hpp>

static inline constexpr glm::vec4 toTexPosSize(const float x, const float y, const float w = 1, const float h = 1) {
    constexpr float TILE_COUNT = 4, FACTOR = 1.f / TILE_COUNT;
    constexpr float PADDING = FACTOR / 1024;
    return {x * FACTOR + PADDING, y * FACTOR + PADDING, w * FACTOR - PADDING * 2, h * FACTOR - PADDING * 2};
}

int main() {
    Coel::Window window({800, 800}, "Platformer Example");
    Coel::SpriteRenderer spriteRenderer;
    float aspect = 1, scale = 3;

    window.onResize = [&aspect, &scale](Coel::Window &w) {
        aspect = (float)w.size.x / w.size.y * scale; //
    };

    window.onMouseScroll = [&scale](Coel::Window &w) {
        scale -= w.mouse.scrollOffset.y / 5;
        if (scale < 0.4) scale = 0.4;
    };

    Coel::create(window);
    Coel::init(spriteRenderer);
    Coel::Renderer::setClearColor(0.24, 0.24, 0.32, 1);
    Coel::Renderer::enableBlend(true);

    Coel::Texture spriteSheet;
    Coel::create(spriteSheet, "Assets/Textures/sheet.png");
    Coel::setMagFilter(spriteSheet, Coel::Nearest);
    Coel::setWrapMode(spriteSheet, Coel::ClampToEdge);

    while (window.isOpen) {
        Coel::Renderer::clearColor();

        float off = window.time - (int)window.time;
        float off2 = window.time / 24 - ((int)window.time) / 24;
        float off3 = window.time / 12 - ((int)window.time) / 12;

        aspect = (float)window.size.x / window.size.y * scale;
        spriteRenderer.resize(aspect, scale);
        spriteRenderer.begin();
        const float s4 = int(scale / 4) + 3;
        const float s1 = int(scale) + 3;
        const glm::vec4 tint = {0.3, 0.3, 0.37, 1.0};
        for (float x = -s4 * 4; x < s4 * 4; x += 4)
            spriteRenderer.submit({x - off3 * 4, 0, 4, 1}, toTexPosSize(0, 2, 4, 1), spriteSheet, tint);
        for (float x = -s4 * 4; x < s4 * 4; x += 4)
            spriteRenderer.submit({x - off2 * 4, 1, 4, 1}, toTexPosSize(0, 3, 4, 1), spriteSheet, tint);
        for (float y = -s1 * 4; y < 0; ++y)
            for (float x = -s1 * 1; x < s1 * 1; ++x)
                spriteRenderer.submit({x - off, y, 1, 1}, toTexPosSize(0, 1), spriteSheet, tint);
        for (float x = -s1 * 1; x < s1 * 1; ++x)
            spriteRenderer.submit({x - off, -0.75, 1, 1}, toTexPosSize(1, 1), spriteSheet, tint);
        spriteRenderer.submit({0, 0, 1, 1}, toTexPosSize((int)(window.time * 4) % 4, 0), spriteSheet);
        for (float x = -s1 * 1; x < s1 * 1; ++x)
            spriteRenderer.submit({x - off, 0, 1, 1}, toTexPosSize(2, 1), spriteSheet, tint);
        spriteRenderer.flush();

        Coel::update(window);
    }

    Coel::deinit(spriteRenderer);
    Coel::destroy(window);
}
