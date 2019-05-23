#include "Coel.hpp"

#define DEBUG 0
#if DEBUG
#include <iostream>
#define LOG(x) std::cout << x
#else
#define LOG(x)
#endif

using namespace Coel;

class MyLayer : public Layer
{
public:
	MyLayer() {}
	~MyLayer() {}

	void onAttach() override {}
	void onDetach() override {}

	void onEvent(Coel::Event &event) override
	{
		Event::dispatch<MouseMovedEvent>([](MouseMovedEvent &e) -> bool {
			LOG("mouse moved " << std::flush);
			return false;
		});
		Event::dispatch<MouseScrolledEvent>([](MouseScrolledEvent &e) -> bool {
			LOG("mouse scrolled " << std::flush);
			return false;
		});
		Event::dispatch<MouseButtonPressedEvent>([](MouseButtonPressedEvent &e) -> bool {
			LOG("mouse pressed " << std::flush);
			return false;
		});
		Event::dispatch<MouseButtonReleasedEvent>([](MouseButtonReleasedEvent &e) -> bool {
			LOG("mouse released " << std::flush);
			return false;
		});
		Event::dispatch<KeyPressedEvent>([](KeyPressedEvent &e) -> bool {
			LOG("key pressed " << std::flush);
			return false;
		});
		Event::dispatch<KeyReleasedEvent>([](KeyReleasedEvent &e) -> bool {
			LOG("key released " << std::flush);
			return false;
		});
		Event::dispatch<WindowResizeEvent>([](WindowResizeEvent &e) -> bool {
			LOG("window resized " << std::flush);
			return false;
		});
		Event::dispatch<WindowCloseEvent>([](WindowCloseEvent& e) -> bool {
			LOG("window closed " << std::flush);
			return false;
		});
		Event::dispatch<AppTickEvent>([](AppTickEvent& e) -> bool {
			LOG('0' << std::flush);
			return false;
		});
		Event::dispatch<AppUpdateEvent>([](AppUpdateEvent& e) -> bool {
			LOG('.' << std::flush);
			return false;
		});
	}
};

class Coeleditor : public Application
{
public:
	Coeleditor()
	{
		pushOverlay(new MyLayer);
	}
	~Coeleditor() {}
};

Application *Application::create()
{
	return new Coeleditor;
}
