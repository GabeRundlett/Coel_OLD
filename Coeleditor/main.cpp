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
}

void onMouseMove(const Coel::MouseMovedEvent &e)
{
}

int main()
{
	Coel::Application::init();

	Coel::setOnTickCallback(&onTick);
	Coel::setOnKeyPressedCallback(&onKeyPress);
	Coel::setOnMouseMovedCallback(&onMouseMove);
	Coel::setOnUpdateCallback(&onUpdate);

	Coel::Application::start();
}
