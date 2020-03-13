#include "Player.hpp"
#include "Random.hpp"
#include "Shaders.hpp"

static Coel::Window window(1600, 960, "Graph");
static Player player;

namespace Planet {
    struct Chunk {
        struct Vertex {
            Math::Vec3 pos;
        };
        static inline ranctx randomContext{};
        static inline constexpr unsigned int LOOP_COUNT = 128, TRIANGLE_COUNT = Math::sq(LOOP_COUNT) * 2,
                                             VERTEX_COUNT = TRIANGLE_COUNT * 3;
        Coel::Vao vao{};
        Coel::Vbo vbo{nullptr, sizeof(Vertex) * VERTEX_COUNT, {{Coel::Element::F32, 3}}};
        Coel::Shader shader{vertSrc, fragSrc};
        unsigned int count{}, level;

        Math::Mat4 modelMat;

        Chunk(const Math::Mat4 &m, const unsigned int i = 0) : level{1U << i}, modelMat{m} {
            vao.add(vbo);
            raninit(&randomContext, 0);
            regen();
        }

        void regen() {
            struct Quad {
                Vertex a1, a2, a3, b1, b2, b3;
            } * quad;
            vbo.open(&quad);
            for (unsigned int yi = 0; yi < LOOP_COUNT; ++yi) {
                for (unsigned int xi = 0; xi < LOOP_COUNT; ++xi) {
                    float x1 = ((0.f + xi) / LOOP_COUNT - 0.5) * 2 / level;
                    float x2 = ((1.f + xi) / LOOP_COUNT - 0.5) * 2 / level;
                    float y1 = ((0.f + yi) / LOOP_COUNT - 0.5) * 2 / level;
                    float y2 = ((1.f + yi) / LOOP_COUNT - 0.5) * 2 / level;

                    Math::Vec3 vertices[]{
                        {x1, y1, 1},
                        {x1, y2, 1},
                        {x2, y1, 1},
                        {x2, y2, 1},
                    };

                    for (auto &v : vertices) {
                        v.normalize();
                        v *= 1.f + 0.01f * (ranval(&randomContext) % 100) / 50;
                    }

                    *quad = {vertices[0], vertices[1], vertices[2], vertices[1], vertices[3], vertices[2]};
                    quad++;
                    ++count;
                }
            }
            vbo.close();
        }
    };

    Chunk chunks[]{
        {Math::Mat4::rotY(0)},
        {Math::Mat4::rotY(Math::PI)},
        {Math::Mat4::rotX(Math::HALF_PI)},
        {Math::Mat4::rotX(-Math::HALF_PI)},
        {Math::Mat4::rotY(Math::HALF_PI)},
        {Math::Mat4::rotY(-Math::HALF_PI)},
    };
} // namespace Planet

int main() {
    window.onKey = [](Coel::Window &w) { player.keyUpdate(w.key); };
    window.onMouseScroll = [](Coel::Window &w) { player.mouseScrollUpdate(w.mouse); };
    window.onMouseMove = [](Coel::Window &w) {
        player.mouseMoveUpdate(w.mouse);
        w.cursorTo(player.mouseCenter);
    };
    window.onResize = [](Coel::Window &w) {
        Coel::Renderer::resizeViewport(0, 0, w.size.x, w.size.y);
        player.windowSizeUpdate(w.size);
        w.cursorTo(player.mouseCenter);
    };
    window.resize();

    Coel::Renderer::enableBlend(true);
    Coel::Renderer::enableDepthTest(true);
    Coel::Renderer::enableCulling(true);

    window.cursorMode(Coel::Window::CursorMode::Disabled);

    player.setPos({0, 0, -4});
    player.setRot({0, 0, 0});

    auto time = window.getTime(), prevTime = time, elapsed = time;
    Coel::Renderer::enableWireframe(false);

    while (window.isOpen()) {
        prevTime = time, time = window.getTime();
        elapsed = time - prevTime;

        Coel::Renderer::clear();

        auto viewMat = player.cam.rotMat;
        viewMat *= player.cam.tranMat;

        for (auto &c : Planet::chunks) {
            c.shader.bind();

            c.shader.sendMat4("projMat", &player.cam.projMat);
            c.shader.sendMat4("viewMat", &viewMat);
            c.shader.sendMat4("modelMat", &c.modelMat);

            c.vao.draw(c.count * 6);
        }

        player.update((float)elapsed);
        window.update();
    }
}
