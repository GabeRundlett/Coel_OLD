#include "pch.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Internals/Utilities/Log.hpp"
#include "Internals/EventCallbacks.hpp"

namespace Coel {
	namespace WindowManager {
		static GLFWwindow *s_window;

		unsigned int init()
		{
			LOG_INFO(WindowManager, "Initializing window manager...\n");
			if (!glfwInit()) {
				LOG_ERROR(WindowManager, "Failed to initialize GLFW\n");
				return 0;
			}
			LOG_INFO(WindowManager, "Creating window...\n");
			s_window = glfwCreateWindow(800, 600, "title", 0, 0);
			if (!s_window) {
				LOG_ERROR(WindowManager, "Failed to create window\n");
				return 0;
			}
			glfwMakeContextCurrent(s_window);
			LOG_SUCCESS(WindowManager, "Created window\n");
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				LOG_ERROR(WindowManager, "Failed to initalize glad\n");
				return 0;
			}
			glfwSwapInterval(0);

			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glCullFace(GL_BACK);

			glfwSetKeyCallback(s_window, [](GLFWwindow *w, int key, int scancode, int action, int mods) {
				switch (action) {
				case GLFW_PRESS:
					Internals::onKeyPressedCallback({key, scancode, mods});
					break;
				case GLFW_RELEASE:
					Internals::onKeyReleasedCallback({key, scancode, mods});
					break;
				case GLFW_REPEAT:
					Internals::onKeyRepeatedCallback({key, scancode, mods});
					break;
				}
			});

			glfwSetMouseButtonCallback(s_window, [](GLFWwindow *w, int button, int action, int mods) {
				switch (action) {
				case GLFW_PRESS:
					Internals::onMousePressedCallback({button, mods});
					break;
				case GLFW_RELEASE:
					Internals::onMouseReleasedCallback({button, mods});
					break;
				}
			});
			glfwSetCursorPosCallback(s_window, [](GLFWwindow *w, double xPos, double yPos) {
				Internals::onMouseMovedCallback({xPos, yPos});
			});
			glfwSetScrollCallback(s_window, [](GLFWwindow *w, double xOffset, double yOffset) {
				Internals::onMouseScrolledCallback({xOffset, yOffset});
			});

			glfwSetWindowPosCallback(s_window, [](GLFWwindow *w, int xPos, int yPos) {
				Internals::onWindowMovedCallback({xPos, yPos});
			});
			glfwSetWindowSizeCallback(s_window, [](GLFWwindow *w, int width, int height) {
				Internals::onWindowResizedCallback({width, height});
			});

			LOG_SUCCESS(WindowManager, "Initialized window manager\n");
			return 1;
		}
		void update()
		{
			glfwPollEvents();
			glfwSwapBuffers(s_window);
		}
		int shouldRun() { return !glfwWindowShouldClose(s_window); }
		void close() { glfwTerminate(); }
	} // namespace WindowManager
#define __ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(name) \
	void setOn##name##Callback(void (*func)(const name##Event &)) { Internals::on##name##Callback = func; }

	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(KeyPressed)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(KeyReleased)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(KeyRepeated)

	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(MouseMoved)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(MousePressed)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(MouseReleased)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(MouseScrolled)

	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(WindowMoved)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(WindowResized)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(WindowClosed)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(WindowFocused)
	__ENGINE_IMPLIMENT_EVENT_CALLBACK_SETTER(WindowLostFocus)
} // namespace Coel
