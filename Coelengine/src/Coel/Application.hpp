#pragma once
#include "Window.hpp"
#include "Layer.hpp"

#include "Graphics/OpenGL/VertexBuffer.hpp"
#include "Graphics/OpenGL/IndexBuffer.hpp"

namespace Coel
{
class Application
{
	static Window *m_window;
	static LayerStack *m_layerStack;

	static Graphics::VertexBuffer *m_vbo;
	static Graphics::IndexBuffer *m_ibo;

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
