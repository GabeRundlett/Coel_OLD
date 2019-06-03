#include "Coel.hpp"

#include <stdio.h>
#include <random>

struct vec2 {
	float x = 0, y = 0;
};

vec2 mouse = {0, 0}, screen = {800, 600};
vec2 mousePressedLoc = mouse;
bool mouseIsPressed = false;

constexpr const unsigned int particleCount = 1000;
constexpr const float particleSize = 40;
unsigned int upt = 0;

struct Particle {
	vec2 pos, vel = {float(rand() % 10000 - 5000) / 200000, float(rand() % 10000 - 5000) / 200000};
	vec2 size = {particleSize / screen.x, particleSize / screen.y};
	inline void draw()
	{
		Coel::Graphics::Renderer::drawRect(pos.x, pos.y, size.x, size.y);
	}
	inline void update()
	{
		size = {particleSize / screen.x, particleSize / screen.y};

		if (pos.x < -1) {
			vel.x *= -0.75;
			pos.x = -1;
		} else if (pos.x > 1 - size.x) {
			vel.x *= -0.75;
			pos.x = 1 - size.x;
		}

		if (pos.y < -1) {
			vel.y *= -0.75;
			pos.y = -1;
		} else if (pos.y > 1 - size.y) {
			vel.y *= -0.75;
			pos.y = 1 - size.y;
		}
		vel.y -= 0.0002;

		pos.x += vel.x;
		pos.y += vel.y;
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

void onKeyPress(const Coel::KeyPressedEvent &e)
{
}

void onMouseMove(const Coel::MouseMovedEvent &e)
{
	mouse = {
		(float)e.xPos / screen.x * 2 - 1,
		(float)e.yPos / screen.y * -2 + 1};
}

void onWindowResize(const Coel::WindowResizedEvent &e)
{
	Coel::Graphics::Renderer::resizeViewport(0, 0, e.width, e.height);
	screen = {(float)e.width, (float)e.height};
}

void onMousePress(const Coel::MousePressedEvent &e)
{
	mouseIsPressed = true;
	for (unsigned int i = 0; i < particleCount; ++i) {
		particles[i].pos = mouse;
		particles[i].vel = {
			float(rand() % 10000 - 5000) / 200000,
			float(rand() % 10000 - 5000) / 200000};
	}
}

void onMouseRelease(const Coel::MouseReleasedEvent &e)
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

	Coel::setOnKeyPressedCallback(&onKeyPress);
	Coel::setOnMouseMovedCallback(&onMouseMove);
	Coel::setOnWindowResizedCallback(&onWindowResize);
	Coel::setOnMousePressedCallback(&onMousePress);
	Coel::setOnMouseReleasedCallback(&onMouseRelease);

	Coel::Application::init();
	Coel::Application::start();
}
