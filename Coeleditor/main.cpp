#include "Coel.hpp"
#include "clm.hpp"

#include <stdio.h>
#include <random>

Coel::vec2 mouse = {0, 0}, screen = {800, 600};
Coel::vec2 mousePressedLoc = mouse;
bool mouseIsPressed = false;

unsigned int particleCount = 200;
constexpr static const unsigned int newParticlesToAdd = 100, particleMaxLife = 1000;
constexpr const float dampening = 0.0, friction = 0.97;
unsigned int upt = 0;

struct Particle {
	Coel::vec2 pos, vel = randomVel();
	Coel::vec2 size = randomSize();
	unsigned int tickLifetime = 0;
	inline void draw()
	{
		if (isAlive())
			Coel::Graphics::Renderer::drawRect(pos.x, pos.y, size.x, size.y);
	}
	inline void update()
	{
		if (isAlive()) {
			if (pos.x < -1) {
				vel.x *= -dampening;
				vel.y *= friction;
				pos.x = -1;
			} else if (pos.x > 1 - size.x) {
				vel.x *= -dampening;
				vel.y *= friction;
				pos.x = 1 - size.x;
			}

			if (pos.y < -1) {
				vel.x *= friction;
				vel.y *= -dampening;
				pos.y = -1;
			} else if (pos.y > 1 - size.y) {
				vel.x *= friction;
				vel.y *= -dampening;
				pos.y = 1 - size.y;
			}
			vel.y -= 9.8 / Coel::Application::TICK_RATE;

			pos.x += vel.x / Coel::Application::TICK_RATE;
			pos.y += vel.y / Coel::Application::TICK_RATE;
		}

		tickLifetime++;
	}
	static Coel::vec2 randomVel()
	{
		return Coel::vec2::polar(float(rand() % 10000 - 5000), float(rand() % 10000) / 2000);
	}
	static Coel::vec2 randomSize()
	{
		return {float(rand() % 10000 + 10000) / 500 / screen.x, float(rand() % 10000 + 10000) / 500 / screen.y};
	}
	inline bool isAlive()
	{
		return tickLifetime < particleMaxLife;
	}
};

Particle *particles;

void onTick()
{
	printf("updates per tick: %d\n", upt);
	upt = 0;
	for (unsigned int i = 0; i < particleCount; ++i)
		particles[i].update();
}

void onUpdate()
{
	for (unsigned int i = 0; i < particleCount; ++i)
		particles[i].draw();
	upt++;
}

void onKeyPress(const Coel::Event::Key::Press &e)
{
}

void onMouseMove(const Coel::Event::Mouse::Move &e)
{
	mouse = {
		(float)e.xPos / screen.x * 2 - 1,
		(float)e.yPos / screen.y * -2 + 1};
}

void onWindowResize(const Coel::Event::Window::Resize &e)
{
	Coel::Graphics::Renderer::resizeViewport(0, 0, e.width, e.height);
	screen = {(float)e.width, (float)e.height};
}

void onMousePress(const Coel::Event::Mouse::Press &e)
{
	mouseIsPressed = true;
	if (e.button == Coel::Mouse::Left) {
		unsigned int deadParticles = 0;
		for (unsigned int i = 0; i < particleCount; ++i) {
			if (!particles[i].isAlive() && deadParticles < newParticlesToAdd) {
				particles[i].pos = mouse;
				particles[i].tickLifetime = 0;
				particles[i].vel = Particle::randomVel();
				deadParticles++;
			}
		}
		if (deadParticles < newParticlesToAdd + 1) {
			Particle *tempParticles = new Particle[particleCount + newParticlesToAdd - deadParticles];
			for (unsigned int i = 0; i < particleCount; ++i)
				tempParticles[i] = particles[i];
			for (unsigned int i = particleCount; i < particleCount + newParticlesToAdd - deadParticles; ++i) {
				tempParticles[i] = Particle();
				tempParticles[i].pos = mouse;
			}
			delete[] particles;
			particles = tempParticles;
			particleCount += newParticlesToAdd - deadParticles;
		}
	}
}

void onMouseRelease(const Coel::Event::Mouse::Release &e)
{
	mouseIsPressed = false;
}

int main()
{
	Coel::setOnTickCallback(&onTick);
	Coel::setOnUpdateCallback(&onUpdate);

	particles = new Particle[particleCount];
	for (unsigned int i = 0; i < particleCount; ++i)
		particles[i] = Particle();

	Coel::setOnKeyPressCallback(&onKeyPress);
	Coel::setOnMouseMoveCallback(&onMouseMove);
	Coel::setOnWindowResizeCallback(&onWindowResize);
	Coel::setOnMousePressCallback(&onMousePress);
	Coel::setOnMouseReleaseCallback(&onMouseRelease);

	Coel::Application::start();

	delete[] particles;
}
