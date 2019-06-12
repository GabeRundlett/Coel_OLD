#include "Coel.hpp"
#include "GUI.hpp"
#include "clm.hpp"

static Coel::vec2 mouse = {0, 0}, screen = {800, 600};
static GUI::Button::Data closeButton = {780, 0, 20, 20, {40, 40, 40, 255}},
						 maximizeButton = {760, 0, 20, 20, {40, 40, 40, 255}},
						 minimizeButton = {740, 0, 20, 20, {40, 40, 40, 255}};

void onTick()
{
}

void onUpdate()
{
	Coel::Graphics::Renderer::drawRect(0, 0, screen.x, 20, Coel::cvec4({40, 40, 40, 255}));
	Coel::cvec4 col1 = {36, 36, 36, 255}, col2 = {50, 50, 50, 255}, col3 = {255, 50, 50, 255};
	GUI::Button::draw(closeButton);
	GUI::Button::draw(maximizeButton);
	GUI::Button::draw(minimizeButton);

	if (GUI::Button::checkIfHovered(closeButton, mouse)) {
		Coel::Graphics::Renderer::drawRect(
			closeButton.x + 1,
			closeButton.y + 1,
			closeButton.w - 2,
			closeButton.h - 2,
			col3);
	} else {
		Coel::Graphics::Renderer::drawRect(
			closeButton.x + 1,
			closeButton.y + 1,
			closeButton.w - 2,
			closeButton.h - 2,
			col1);
	}
	if (GUI::Button::checkIfHovered(maximizeButton, mouse)) {
		Coel::Graphics::Renderer::drawRect(
			maximizeButton.x + 1,
			maximizeButton.y + 1,
			maximizeButton.w - 1,
			maximizeButton.h - 2,
			col2);
	} else {
		Coel::Graphics::Renderer::drawRect(
			maximizeButton.x + 1,
			maximizeButton.y + 1,
			maximizeButton.w - 1,
			maximizeButton.h - 2,
			col1);
	}
	if (GUI::Button::checkIfHovered(minimizeButton, mouse)) {
		Coel::Graphics::Renderer::drawRect(
			minimizeButton.x + 1,
			minimizeButton.y + 1,
			minimizeButton.w - 1,
			minimizeButton.h - 2,
			col2);
	} else {
		Coel::Graphics::Renderer::drawRect(
			minimizeButton.x + 1,
			minimizeButton.y + 1,
			minimizeButton.w - 1,
			minimizeButton.h - 2,
			col1);
	}
}

void onMouseMove(const Coel::Event::Mouse::Move &e)
{
	mouse = {(float)e.xPos, (float)e.yPos};
}

void onWindowResize(const Coel::Event::Window::Resize &e)
{
	screen = {(float)e.width, (float)e.height};
	closeButton.x = screen.x - 20;
	maximizeButton.x = screen.x - 40;
	minimizeButton.x = screen.x - 60;
}

void onMousePress(const Coel::Event::Mouse::Press &e)
{
	GUI::Button::checkIfPressed(closeButton, mouse);
	GUI::Button::checkIfPressed(closeButton, mouse);
	GUI::Button::checkIfPressed(closeButton, mouse);
}

void onClosePress()
{
	Coel::Application::close();
}
void onMaximizePress()
{
	Coel::Application::maximize();
}
void onMinimizePress()
{
	Coel::Application::minimize();
}

int main()
{
	GUI::Button::setPressCallback(closeButton, onClosePress);
	GUI::Button::setPressCallback(maximizeButton, onMaximizePress);
	GUI::Button::setPressCallback(minimizeButton, onMinimizePress);

	Coel::setOnTickCallback(&onTick);
	Coel::setOnUpdateCallback(&onUpdate);

	Coel::setOnMouseMoveCallback(&onMouseMove);
	Coel::setOnWindowResizeCallback(&onWindowResize);
	Coel::setOnMousePressCallback(&onMousePress);

	Coel::Application::start();
}
