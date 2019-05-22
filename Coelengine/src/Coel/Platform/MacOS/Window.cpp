#include "clpch.hpp"
#include "Window.hpp"
#include "Coel/Graphics/OpenGL/Context.hpp"

#include <GLFW/glfw3.h>
#include "Coel/Utilities/Log.hpp"

namespace Coel
{
Window *Window::create(const WindowProp &prop)
{
	return new Platform::MacOS::Window(prop);
}
namespace Platform
{
namespace MacOS
{
Window::Window(const WindowProp &prop)
	: m_window(nullptr), m_data(prop), m_Context(nullptr)
{
	CL_LOG("Creating window...\n");
	CL_ASSERT(glfwInit(), "Unable to initialize glfw.\n");

	m_window = glfwCreateWindow(800, 600, "window", nullptr, nullptr);
	CL_BREAK(m_window, "Unable to create window.\n");

	m_Context = new Graphics::OpenGL::Context(m_window);
	m_Context->init();

	glfwSetWindowUserPointer(m_window, &m_data);

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow *window, int width, int height) {
		WindowProp &data = *(WindowProp *)glfwGetWindowUserPointer(window);
		data.width = width;
		data.height = height;

		WindowResizeEvent e(width, height);
		data.eventCallback(e);
	});
	glfwSetWindowCloseCallback(m_window, [](GLFWwindow *window) {
		WindowProp &data = *(WindowProp *)glfwGetWindowUserPointer(window);

		WindowCloseEvent e;
		data.eventCallback(e);
	});
	glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
		WindowProp &data = *(WindowProp *)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
		{
			KeyPressedEvent e(key, 0);
			data.eventCallback(e);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleasedEvent e(key);
			data.eventCallback(e);
			break;
		}
		case GLFW_REPEAT:
		{
			KeyPressedEvent e(key, 1);
			data.eventCallback(e);
			break;
		}
		}
	});
	glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, int button, int action, int mods) {
		WindowProp &data = *(WindowProp *)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent e(button);
			data.eventCallback(e);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent e(button);
			data.eventCallback(e);
			break;
		}
		}
	});
	glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double xPos, double yPos) {
		WindowProp &data = *(WindowProp *)glfwGetWindowUserPointer(window);

		MouseMovedEvent e((float)xPos, (float)yPos);
		data.eventCallback(e);
	});
	glfwSetScrollCallback(m_window, [](GLFWwindow *window, double xOffset, double yOffset) {
		WindowProp &data = *(WindowProp *)glfwGetWindowUserPointer(window);

		MouseScrolledEvent e((float)xOffset, (float)yOffset);
		data.eventCallback(e);
	});

	CL_LOG("Successfully created window.\n");
}

Window::~Window()
{
	delete m_Context;
}

bool Window::closed()
{
	return glfwWindowShouldClose(m_window);
}

void Window::update()
{
	glfwPollEvents();
	m_Context->swap();
}
} // namespace MacOS
} // namespace Platform
} // namespace Coel