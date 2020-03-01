#include <Coel.hpp>
#include <iostream>
#include <random>

#include "Shaders.hpp"

struct Particle {
    Math::Vec2 pos, vel;
    float mass;

    bool isInCollision;
};

constexpr float GRAV = 2.f;

float random() { return float(rand() % 1000) / 1000; }

int main() {
    Coel::Window window(1000, 1000, "Physics");
    Coel::Renderer::Batch2d renderer;
    Coel::Shader shader(vertSrc, fragSrc);

    Coel::Renderer::Command::setClearColor(0.9f, 0.7f, 0.2f, 1.f);
    renderer.fill(0xff00ffff);

    Particle particles[10] /* = {
         {{-0.5, 0}, {0, 0.01}, 0.05},
         {{0.5, 0}, {0, -0.01}, 0.05},
     }*/;
    for (auto &p : particles)
        p = {
            {(random() - 0.5f) * 2, (random() - 0.5f) * 2},
            {(random() - 0.5f) * 0.5f, (random() - 0.5f) * 0.5f},
            random() / 10,
            false,
        };

    // particles[0] = {
    //     {0, 0},
    //     {0, 0},
    //     0.5
    // };

    auto time = window.getTime(), prevTime = time, timeStep = 0.1;

    while (window.isOpen()) {
        Coel::Renderer::Command::clear();
        prevTime = time;
        time = window.getTime();
        auto elapsed = time - prevTime;

        for (auto i = 0; i < 1; ++i) {
            for (auto &p1 : particles) {
                for (auto &p2 : particles) {
                    if (&p1 != &p2) {
                        auto deltaPos = p2.pos - p1.pos;
                        auto distance = deltaPos.length();

                        auto oneOverDist = 1.f / distance;
                        auto normal = deltaPos * oneOverDist;

                        auto minDist = p1.mass + p2.mass;

                        if (distance < minDist) { // deflect
                            auto distFac = (minDist - distance) * 0.5f;

                            // if (p1.mass > p2.mass) {
                            //     p1.mass += Math::sq(p2.mass) / p1.mass / 6.28;
                            //     p2.isAlive = false;
                            // } else {
                            //     p2.mass += Math::sq(p1.mass) / p2.mass / 6.28;
                            //     p1.isAlive = false;
                            // }

                            p1.pos -= normal * distFac;
                            p2.pos += normal * distFac;

                            constexpr auto COEF_REST = 1.f;

                            auto v1 = p1.vel.dot(normal);
                            auto v2 = p2.vel.dot(normal);
                            
                            auto u1 = v1 * p1.mass;
                            auto u2 = v2 * p2.mass;

                            auto totalMass = p1.mass + p2.mass;

                            auto va = ((v2 - v1) * COEF_REST * p1.mass + u1 + u2) / totalMass;
                            auto vb = ((v1 - v2) * COEF_REST * p2.mass + u1 + u2) / totalMass;

                            p1.vel -= normal * va / 2;
                            p2.vel -= normal * vb / 2;

                            p1.isInCollision = true;
                            p2.isInCollision = true;
                        } else { // gravitate
                            p1.vel += normal * Math::sq(oneOverDist) * GRAV * p2.mass * elapsed;
                            p1.isInCollision = false;
                            p2.isInCollision = false;
                        }
                    }
                }
                p1.pos += p1.vel * elapsed;
            }

            for (auto &p : particles) {
                if (p.pos.x > 1.f - p.mass)
                    p.pos.x = 1.f - p.mass, p.vel.x *= -1;
                else if (p.pos.x < -1.f + p.mass)
                    p.pos.x = -1.f + p.mass, p.vel.x *= -1;
                if (p.pos.y > 1.f - p.mass)
                    p.pos.y = 1.f - p.mass, p.vel.y *= -1;
                else if (p.pos.y < -1.f + p.mass)
                    p.pos.y = -1.f + p.mass, p.vel.y *= -1;
            }
        }

        for (auto &p : particles) {
            if (p.isInCollision)
                renderer.fill(0xff0000ff);
            else
                renderer.fill(0xffffffff);
            renderer.submitEllipse(p.pos.x, p.pos.y, p.mass, p.mass);
        }

        renderer.flush();
        window.update();
    }
}
