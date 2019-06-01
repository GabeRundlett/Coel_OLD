#include "Engine.hpp"
#include <stdio.h>

static unsigned int upt = 0;

void onTick()
{
	//printf("%d updates per tick\n", upt);
	upt = 0;
}

void onUpdate()
{
	upt++;
}

void onKeyPress(const Engine::KeyPressedEvent &e)
{
	printf("Key pressed!  ");
}

void onMouseMove(const Engine::MouseMovedEvent &e)
{
	printf("Mouse moved to (%f, %f)\n", e.xOffset, e.yOffset);
}

int main()
{
	Engine::setOnTickCallback(&onTick);
	Engine::setOnKeyPressedCallback(&onKeyPress);
	Engine::setOnMouseMovedCallback(&onMouseMove);
	Engine::setOnUpdateCallback(&onUpdate);

	Engine::Application::init();
	Engine::Application::start();
}
