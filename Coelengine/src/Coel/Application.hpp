#pragma once
#include "Window.hpp"
#include "Layer.hpp"

namespace Coel
{
class Application
{
	static Window *m_window;
	static LayerStack *m_layerStack;

	static unsigned int m_vao, m_vbo, m_ibo;

public:
	static Application *create();
	Application();
	virtual ~Application();
	static bool closed();
	static void run();
	static void onEvent(Event &e);
	static void pushLayer(Layer *l);
	static void pushOverlay(Layer *l);
};
} // namespace Coel