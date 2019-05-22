#include "clpch.hpp"
#include "Application.hpp"
#include "Utilities/Log.hpp"
#include "Utilities/Timer.hpp"

#include <glad/glad.h>

namespace Coel
{
Event *Event::s_current = nullptr;
Window *Application::m_window = nullptr;
LayerStack *Application::m_layerStack = nullptr;

Graphics::VertexBuffer *Application::m_vbo = nullptr;
Graphics::IndexBuffer *Application::m_ibo = nullptr;

Application::Application()
{
	CL_LOG("Creating Coel Application\n");

	m_window = Window::create();
	CL_BREAK(m_window, "Window creation failed.\n");
	m_window->setEventCallback(onEvent);

	m_layerStack = new LayerStack;
	CL_LOG("Successfully created Coel Application\n");

	float pos[] = { -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f };
	unsigned int ind[] = { 0, 1, 2 };

	m_vbo = new Graphics::OpenGL::VertexBuffer(pos, 3, 2);
	m_ibo = new Graphics::OpenGL::IndexBuffer(ind, 3);
}

Application::~Application()
{
	delete m_layerStack;
	delete m_window;
	delete m_vbo, m_ibo;
}

bool Application::closed()
{
	return m_window->closed();
}

void Application::run()
{
	constexpr const unsigned int TICK_RATE = 128;
	Timer tickClock;
	const float tickTime = 1.f / TICK_RATE;
	float tickOffset = 0, updateOffset = 0;

	while (!m_window->closed())
	{
		AppUpdateEvent updateEvent(tickClock.elapsed() - updateOffset);
		updateOffset = tickClock.elapsed();
		onEvent(updateEvent);

		float sinceLastTick = tickClock.elapsed() - tickOffset;
		if (sinceLastTick > tickTime)
		{
			AppTickEvent tickEvent(sinceLastTick);
			tickOffset += tickTime;
			onEvent(tickEvent);
		}
		
		m_vbo->bind();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		m_window->update();
	}
}

void Application::onEvent(Event &e)
{
	Event::s_current = &e;

	for (auto it = m_layerStack->end(); it != m_layerStack->begin();)
	{
		(*--it)->onEvent(e);
		if (e.handled)
			break;
	}
}

void Application::pushLayer(Layer *l)
{
	m_layerStack->pushLayer(l);
}

void Application::pushOverlay(Layer *l)
{
	m_layerStack->pushOverlay(l);
}
} // namespace Coel
