#include <Coel.hpp>

glm::vec2 mouse{};
inline constexpr float sq(float x) { return x * x; }
struct Particle {
    glm::vec2 pos{}, vel{};
    static inline constexpr float lifetime = 5;
    float time{randf() * lifetime};
    float size{0}, dir{};

    float randf() { return 0.001f * (std::rand() % 1000); }

    void reset() {
        size = randf();
        dir = randf() * 6.28;
        time = 0;
        const float r = (1 - sq(randf())) * 3;
        const float s = std::sin(dir), c = std::cos(dir);
        vel = {r * c, r * s}, pos = vel * 0.2f + mouse;
    }

    void update(float elapsed) {
        pos += vel * elapsed / (time * 2 + 1);
        time += elapsed;
        if (time > lifetime) reset();
    }
};
std::vector<Particle> particles{10000};

int main() {
    Coel::Window window("Platformer Example");
    Coel::SpriteRenderer spriteRenderer;
    float aspect = 1, scale = 4;

    window.onResize = [&aspect, &scale](Coel::Window &w) {
        aspect = (float)w.size.x / w.size.y * scale; //
    };

    window.onMouseScroll = [&scale](Coel::Window &w) {
        scale -= w.mouse.scrollOffset.y / 5;
        if (scale < 0.4) scale = 0.4;
    };

    window.onMouseMove = [&scale](Coel::Window &w) {
        mouse.x = (w.mouse.pos.x - 0.5 * w.size.x) / w.size.y * scale * 2.0;
        mouse.y = (0.5 * w.size.y - w.mouse.pos.y) / w.size.y * scale * 2.0;
    };

    Coel::create(window);
    Coel::init(spriteRenderer);
    Coel::Renderer::setClearColor(0.1, 0.1, 0.1, 1);

    Coel::Texture fire, smoke;
    Coel::create(smoke, "Assets/Textures/Sprites/smoke_particle.png");
    Coel::create(fire, "Assets/Textures/Sprites/fire_particle.png");
    Coel::setMagFilter(smoke, Coel::Nearest);
    Coel::setMagFilter(fire, Coel::Nearest);
    Coel::Renderer::enableBlend(true);

    auto time = Coel::getTime(), prevTime = time, elapsed = time;

    {
        Coel::Renderer::ImGuiRenderer imgui(window);

        while (window.isOpen) {
            prevTime = time, time = Coel::getTime();
            elapsed = time - prevTime;
            Coel::Renderer::clearColor();

            aspect = (float)window.size.x / window.size.y * scale;
            spriteRenderer.resize(aspect, scale);
            spriteRenderer.begin();

            spriteRenderer.submit({0, 0, 10, 10}, {0.4, 0.5, 0.84, 1});
            for (auto &p : particles) {
                const float s = p.size / (p.time + 2);
                const glm::vec4 pos_size = {p.pos.x, p.pos.y, s + 0.1, s + 0.1};
                if (p.time < 1)
                    spriteRenderer.submit(pos_size, p.dir + s * 4, fire, {1, 1 - p.time / p.lifetime, 1, sq(s) * 5});
                else
                    spriteRenderer.submit(pos_size, p.dir + s * 4, smoke, {1, 1, 1, sq(s) * 5});
                p.update(elapsed);
            }

            spriteRenderer.flush();

            imgui.begin();
            ImGui::Begin("[Coelengine] Sprite Renderer Info: ");
            ImGui::Text("draw calls: %d", spriteRenderer.drawCallCount);
            ImGui::Text("submissions: %d", spriteRenderer.submissionCount);
            ImGui::DragFloat("Zoom scale", &scale);
            ImGui::End();
            ImGui::ShowMetricsWindow();
            imgui.end(window);

            spriteRenderer.drawCallCount = 0;
            spriteRenderer.submissionCount = 0;

            Coel::update(window);
        }
    }

    Coel::deinit(spriteRenderer);
    Coel::destroy(window);
}
