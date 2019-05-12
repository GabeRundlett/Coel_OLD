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

unsigned int Application::m_vao = 0, Application::m_vbo = 0, Application::m_ibo = 0;

Application::Application()
{
	CL_LOG("Creating application...\n");

	m_window = Window::create();
	CL_BREAK(m_window, "Window creation failed.\n");
	m_window->setEventCallback(onEvent);

	m_layerStack = new LayerStack;
	CL_LOG("successfully created application...\n");
	
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	float pos[] = {
		-0.5f, -0.5f, 0.f,
		 0.5f, -0.5f, 0.f,
		 0.0f,  0.5f, 0.f,
	};
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	
	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	
	unsigned int ind[] = {
		0, 1, 2
	};
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
}

Application::~Application()
{
	delete m_layerStack;
	delete m_window;
}

bool Application::closed()
{
	return m_window->closed();
}

void Application::run()
{
	Timer frameTimer;
	float frameCount = 0;
	while (!m_window->closed())
	{
		if (frameTimer.elapsed() > 5)
		{
			CL_LOG(frameCount / 5 << " fps\n");
			frameTimer.reset();
			frameCount = 0;
		}
		frameCount++;
		
		glBindVertexArray(m_vao);
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
