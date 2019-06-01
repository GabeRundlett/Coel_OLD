#include "Coel.hpp"
#include <stdio.h>

void onTick()
{
}

void onUpdate()
{
}

void onKeyPress(const Coel::KeyPressedEvent &e)
{
	printf("Key pressed! ");
}

void onMouseMove(const Coel::MouseMovedEvent &e)
{
	printf("Mouse moved to (%f, %f)\n", e.xOffset, e.yOffset);
}

int main()
{
	Coel::setOnTickCallback(&onTick);
	Coel::setOnKeyPressedCallback(&onKeyPress);
	Coel::setOnMouseMovedCallback(&onMouseMove);
	Coel::setOnUpdateCallback(&onUpdate);

	Coel::Application::init();
	Coel::Application::start();
}
