#pragma once
#include "Coel/Window.hpp"
#include "Coel/Graphics/Context.hpp"

struct GLFWwindow;

namespace Coel
{
namespace Platform
{
namespace MacOS
{
class Window : public Coel::Window
{
	GLFWwindow *m_window;
	WindowProp m_data;
	Graphics::Context *m_Context;

public:
	Window(const WindowProp &prop);
	~Window();
	bool closed() override;
	void update() override;
	unsigned int getWidth() override { return m_data.width; };
	unsigned int getHeight() override { return m_data.height; };
	void setEventCallback(void (*func)(Event &)) { m_data.eventCallback = func; };
};
} // namespace MacOS
} // namespace Platform
} // namespace Coel
