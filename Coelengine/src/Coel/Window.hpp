#pragma once
#include "Events/Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/AppEvent.hpp"

namespace Coel
{
struct WindowProp
{
	const char *title;
	unsigned int width, height;
	void (*eventCallback)(Event &);

	WindowProp(const char *t = "Coel App",
			   unsigned int w = 800, unsigned int h = 600)
		: title(t), width(w), height(h), eventCallback(nullptr) {}
};

class Window
{
public:
	static Window *create(const WindowProp &prop = WindowProp());
	virtual ~Window() {}
	virtual bool closed() = 0;
	virtual void update() = 0;
	virtual unsigned int getWidth() = 0;
	virtual unsigned int getHeight() = 0;
	virtual void setEventCallback(void (*func)(Event &)) = 0;
};
} // namespace Coel