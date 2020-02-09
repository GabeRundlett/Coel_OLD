#include <Coel.hpp>
#include <vector>

static constexpr const char *const vertSrc = R"(
#version 440
layout (location = 0) in vec2 pos;
void main() {
	gl_Position = vec4(pos, 0, 1);
})";
static constexpr const char *const fragSrc = R"(
#version 440
out vec4 color;
void main() {    
    color = vec4(0.1, 0.1, 0.1, 1);
})";

struct Particle {
    Math::Vec2 pos, vel;

    void update(float elapsed) {
        constexpr float COLLISION_DAMPENING = 0.3, COLLISION_FRICTION = 0.1, AIR_RESISTANCE = 0.001;
        constexpr float D1 = 1.f - COLLISION_DAMPENING, D2 = 1.f - COLLISION_FRICTION, D3 = 1.f - AIR_RESISTANCE;

        if (pos.x < -1)
            vel.x *= -D1, vel.y *= D2, pos.x = -1;
        else if (pos.x > 1)
            vel.x *= -D1, vel.y *= D2, pos.x = 1;

        if (pos.y < -1)
            vel.x *= D2, vel.y *= -D1, pos.y = -1;
        else if (pos.y > 1)
            vel.x *= D2, vel.y *= -D1, pos.y = 1;

        vel.y += -10.f * elapsed;
        pos += vel * elapsed;
        vel *= D3;
    }
};

float random() { return float(rand() % 1000) / 1000; }

int main() {
    Coel::Window window(1000, 1000, "Simple Batch Rendering Example");

    constexpr unsigned int PARTICLE_COUNT = 1000;
    std::vector<Particle> particles(PARTICLE_COUNT);

    Coel::Renderer::Batch2d renderer;
    Coel::Renderer::Command::setClearColor(0.2f, 0.4f, 0.8f, 1.f);

    for (unsigned int i = 0; i < PARTICLE_COUNT; ++i) {
        particles[i].pos = {0.f, 0.f};
        particles[i].vel = {random() - 0.5f, random() - 0.5f};
        particles[i].vel *= 5;
    }

    Coel::Shader shader(vertSrc, fragSrc);

    double time = window.getTime(), prevTime;

    while (window.isOpen()) {
        prevTime = time;
        time = window.getTime();
        double elapsed = time - prevTime;

        Coel::Renderer::Command::clear();
        shader.bind();

        for (int i = 0; i < PARTICLE_COUNT; ++i) {
            auto &p = particles[i];
            float l = p.vel.length();
            Math::Vec2 size = {0.05, 0.05};
            renderer.submitRect(p.pos.x - size.x / 2, p.pos.y - size.y / 2, size.x, size.y);
            p.update(elapsed);
        }

        renderer.flush();
        window.update();
    }
}
// 62.51

// 75.06 - 72.42
// 73.19 - 71.42
