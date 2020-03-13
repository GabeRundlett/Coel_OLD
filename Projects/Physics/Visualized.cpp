#include <Coel.hpp>

#include "Random.hpp"

static Coel::Window window(1600, 960, "Physics Visualized");
static Coel::Renderer::Batch2d batch;
static Math::Vec2 graphPos{20, 100};
static ranctx randomContext;
float graphOffset = 0;

int main() {
    raninit(&randomContext, 0);

    window.onResize = [](Coel::Window &w) { batch.resize({0, 0}, w.size); };
    window.resize();

    Coel::Renderer::enableBlend(true);
    Coel::Renderer::setClearColor(0.1, 0.1, 0.1, 1);
    batch.strokeWeight(2);

    auto graphLine = [](const auto fpsGen, auto... args) {
        float time = 0.f, pos = 0.f, prevPos = pos, vel = 1.f;
        const float acc = 0.001f * 0;
        while (time < 1500) {
            const float timestep = 100.f / fpsGen(args...);
            prevPos = pos, pos += vel * timestep, vel += acc * timestep;
            batch.submitLine(time + graphPos.x, prevPos + graphPos.y, time + timestep + graphPos.x, pos + graphPos.y);
            time += timestep;
            vel *= 1.f - timestep / 100 * 0.1;
        }
    };

    while (window.isOpen()) {
        Coel::Renderer::clearColor();

        // graphOffset = (float)window.mouse.pos.x / 50;

        batch.fill(0xff000080);
        graphPos = {20, 50 + graphOffset};
        graphLine([] { return 5; });
        batch.fill(0x0000ff40);
        graphPos = {20, 50};
        graphLine([] { return 500; });
        batch.fill(0x00ff0040);
        graphPos = {20, 50 - graphOffset};
        graphLine([] { return ranval(&randomContext) % 500 + 1; });

        batch.flush();
        window.update();
    }
}
